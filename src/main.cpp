#include <iostream>
#include <ostream>
#include <vector>
#include "board.h"
#include "open_ai.h"
using namespace std;

int main() {
    // Declare variables to hold the user's input
    string name;
    short player; //1 or 2

    // Prompt the user for their name
    cout << "Enter your name: ";
    getline(cin, name); // Read a line of text including spaces

    cout << "Which player do you want to be? (1 or 2)\n";
    cin >> player;
    // Discard the newline character left in the input buffer
    cin.ignore(numeric_limits<streamsize>::max(), '\n');

    Board board(3,3);
    board.print();

    int currentPlayer = 1;
    for(int i = 0; i < 9; i++) {
        int w = board.winner();
        if(w == player) {
            cout << "Congrats, " << name <<  ", you won the game!" << endl;
            break;
        }
        else if(w != 0) {
            cout << "You have lost the game." << endl;
            break;
        }
        else {
            int col, row;
            if(currentPlayer == player) {
                // Let the player decide on the next move
                cout << "Player " << currentPlayer << ", enter the row of your choice (1-3): ";
                cin >> row;
                cout << "Player " << currentPlayer << ", enter the column of your choice (1-3): ";
                cin >> col; 
                row--; col--;
            }
            else {
                // Make a request to open AI to determine the next move of the AI player
                ostringstream userMessage;
                userMessage << "The current board is " << board.to_string() << ". ";
                userMessage << "You are player " << currentPlayer << ". ";
                userMessage << "Make your move.";
                vector<int> position = request("src/tic_request.json", userMessage.str());
                row = position[0]; col = position[1];
            }
            if(board.get(row, col) == 0) {
                // If the requested position is empty, update the board and current player
                board.set(row, col, currentPlayer);
                cout << "The current board is:\n";
                board.print();
                if(currentPlayer == 1)
                    currentPlayer = 2;
                else currentPlayer = 1;
            }
            else {
                cout << "Position [" << ++row << "," << ++col << "] is already occupied." << endl;
            }
        }
    }

    cout << "Thank you for playing!" << endl;

    return 0; // Indicate that the program ended successfully
}