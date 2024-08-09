#include <iostream>
#include <ostream>
#include <vector>
#include "board.h"
#include "open_ai.h"
using namespace std;

string print_winner(int x, string name1, string name2) {
    if (x == 1)
        return name1;
    else return name2;
}

int main() {
    string response = request();
    cout << response << endl;
    // Declare variables to hold the user's input
    string name1, name2;
    int age;
    // Prompt the user for their name
    cout << "Enter the name of player 1: ";
    getline(cin, name1); // Read a line of text including spaces

    cout << "Enter the name of player 2: ";
    getline(cin, name2);

    Board board(3,3);
    board.print();

    int currentPlayer = 1;
    for(int i = 0; i < 9; i++) {
        int w = board.winner();
        if(w != 0) {
            cout << "Congrats, " << print_winner(w, name1, name2) <<  ", you won the game!" << "\n";
            break;
        }
        else {
            int col, row;
            cout << "Player " << currentPlayer << ", enter the row of your choice (1-3): ";
            cin >> row;
            cout << "Player " << currentPlayer << ", enter the column of your choice (1-3): ";
            cin >> col; 
            if(board.get(--row, --col) == 0) {
                board.set(row, col, currentPlayer);
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