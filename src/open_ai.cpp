#include "open_ai.h"

/* Reads JSON data from a file */
static ptree read_json_file(const string& jsonFilePath) {
    ptree content;

    try {
        read_json(jsonFilePath, content);
    } catch (exception &e) {
        cerr << "Error reading JSON file: " << e.what() << endl;
    }

    return content;
}

/* Reads the open AI API key from the 'secret' config file */
static string read_open_ai_key() {
    ptree config = read_json_file("config.json");

    // Try accessing the open ai API key
    try {
        string apiKey = config.get<std::string>("open_ai_api_key");
        return apiKey;

    } catch (const ptree_bad_path &e) {
        cerr << "Failed to find the open ai API key in the configuration file: " << e.what() << endl;
    }
    return "";
}

/* The property tree library converts integers into strings.
   To comply with the structure of an open ai request, the 
   required integer in max tokens will be manually converted. */
static string set_max_tokens_integer(const string& jsonString) {
    regex search("\"max_tokens\":\"30\"");
    string replace = "\"max_tokens\":30";
    return regex_replace(jsonString, search, replace);
}

/* Convert JSON object 'jsonData' into a string, sucht that it 
   can be used in an API request */
static string json_to_string(ptree jsonData) {
    jsonData.put("max_tokens", 30);
    stringstream jsonStringStream;

    try {
        write_json(jsonStringStream, jsonData, false);
    } catch (exception &e) {
    cerr << "Error converting JSON to string: " << e.what() << endl;
    return "";
    }

    string jsonString = set_max_tokens_integer(jsonStringStream.str());
    return jsonString;
}

/* Prepares content for an openAI user message */
static ptree create_user_message(const string& content) {
    ptree newMessage;
    newMessage.put("role", "user");
    newMessage.put("content", content);

    return newMessage;
}

/* Appends content as a user message to the messages array used in
   openAI API requests. */
static ptree append_user_message(ptree apiData, const string& content) {
    // Format user message
    ptree userMessage = create_user_message(content);

    // Append user message to the messages in apiData
    ptree& messages = apiData.get_child("messages");
    messages.push_back(make_pair("", userMessage));

    return apiData;
}

/* Callback function to handle the response data */
static size_t WriteCallback(void* contents, size_t size, size_t nmemb, void* userp) {
    ((string*)userp)->append((char*)contents, size * nmemb);
    return size * nmemb;
}

/* Parses the arguments returned by the 'set_board' function */
static vector<int> parse_arguments(string args) {
    vector<int> pos(2);
    istringstream is(args);
    ptree pt; 
    read_json(is, pt); 

    pos[0] = pt.get<int>("row");
    pos[1] = pt.get<int>("column");

    return pos;
}

/* Parses the response returned by the openAI API */
static string parse_response(string& raw_response) {
    istringstream is(raw_response);
    ptree pt;
    string content;

    try {
        read_json(is, pt);

        // Access the "choices" array
        ptree choicesArray = pt.get_child("choices");

        // Iterate through the array (only one item in this case)
        for (auto& item : choicesArray) {
            ptree message = item.second.get_child("message");
            ptree toolCallsArray = message.get_child("tool_calls");

            // Again iterate through the array that contains only one item
            for (auto& tool : toolCallsArray) {
                ptree function = tool.second.get_child("function");
                content = function.get<string>("arguments");
            }
        }
    }
    catch (json_parser_error &e) {
        cerr << "Failed to parse JSON: " << e.what() << endl;
    } 
    catch (ptree_bad_path &e) {
        cerr << "Failed to find the path in JSON: " << e.what() << endl;
    }
    return content;
}

/* Prepares and send requests to the openAI API. The base request
   is stored as a .json file at the filePath. A usermessage is
   be appended to the base request to provide full context. */
vector<int> request(string filePath, string userMessage) {
    CURL* curl;
    CURLcode res;
    string readBuffer;
    vector<int> response;

    curl_global_init(CURL_GLOBAL_ALL);
    curl = curl_easy_init();
    if(curl) {
        struct curl_slist* headers = NULL;
        headers = curl_slist_append(headers, "Content-Type: application/json");
        string fullHeader = string("Authorization: Bearer ") + read_open_ai_key();
        headers = curl_slist_append(headers, fullHeader.c_str());

        curl_easy_setopt(curl, CURLOPT_URL, "https://api.openai.com/v1/chat/completions");
        curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);

        // Prepare JSON data to send in request
        ptree baseMessage = read_json_file(filePath);
        ptree fullMessage = append_user_message(baseMessage, userMessage);
        string jsonData = json_to_string(fullMessage);

        curl_easy_setopt(curl, CURLOPT_POSTFIELDS, jsonData.c_str());
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &readBuffer);

        res = curl_easy_perform(curl);
        if(res != CURLE_OK) {
            cerr << "curl_easy_perform() failed: " << curl_easy_strerror(res) << endl;
        } else {
            response = parse_arguments(parse_response(readBuffer));
        }

        curl_easy_cleanup(curl);
        curl_slist_free_all(headers);
    }

    curl_global_cleanup();
    return response;
}