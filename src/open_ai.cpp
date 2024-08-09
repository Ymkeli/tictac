#include "open_ai.h"

string read_open_ai_key() {
    string configFilePath = "config.json";
    ptree config;

    // Try reading the JSON file
    try {
        read_json(configFilePath, config);
    } catch (const json_parser_error &e) {
        cerr << "Failed to read the configuration file: " << e.what() << endl;
    }

    // Try accessing the open ai API key
    try {
        string apiKey = config.get<std::string>("open_ai_api_key");
        return apiKey;

    } catch (const ptree_bad_path &e) {
        cerr << "Failed to find the open ai API key in the configuration file: " << e.what() << endl;
    }
    return "";
}

// Callback function to handle the response data
size_t WriteCallback(void* contents, size_t size, size_t nmemb, void* userp) {
    ((string*)userp)->append((char*)contents, size * nmemb);
    return size * nmemb;
}

string parse_response(string raw_response) {
    istringstream is(raw_response);
    ptree pt;
    string content;

    try {
        read_json(is, pt);

        // Access the "choices" array
        ptree choicesArray = pt.get_child("choices");

        // Iterate through the array (only one item in this case)
        for (auto& item : choicesArray) {
            // Access the "message" object within the current item
            ptree message = item.second.get_child("message");

            // Get the "content" field
            content = message.get<string>("content");
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

string request() {
    CURL* curl;
    CURLcode res;
    string readBuffer;
    string response;

    curl_global_init(CURL_GLOBAL_ALL);
    curl = curl_easy_init();
    if(curl) {
        struct curl_slist* headers = NULL;
        headers = curl_slist_append(headers, "Content-Type: application/json");
        string fullHeader = string("Authorization: Bearer ") + read_open_ai_key();
        headers = curl_slist_append(headers, fullHeader.c_str());

        curl_easy_setopt(curl, CURLOPT_URL, "https://api.openai.com/v1/chat/completions");
        curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);

        // JSON data to send
        string jsonData = R"({
            "model": "gpt-4o-mini",
            "messages": [
                {"role": "system", "content": "You are a helpful assistant."},
                {"role": "user", "content": "Translate the following English text to French: 'Hello, world!'"}
            ],
            "max_tokens": 60
        })";

        curl_easy_setopt(curl, CURLOPT_POSTFIELDS, jsonData.c_str());
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &readBuffer);

        res = curl_easy_perform(curl);
        if(res != CURLE_OK) {
            cerr << "curl_easy_perform() failed: " << curl_easy_strerror(res) << endl;
        } else {
            response = parse_response(readBuffer);
        }

        curl_easy_cleanup(curl);
        curl_slist_free_all(headers);
    }

    curl_global_cleanup();
    return response;
}