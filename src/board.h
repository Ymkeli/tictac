#ifndef BOARD_H
#define BOARD_H

#include <vector>
#include <iostream>
#include <sstream>
using namespace std;

class Board {
private:
    vector<vector<int> > grid;
    int rows;
    int cols;

public:
    // Constructor
    Board(int rows, int cols);

    // Method to print the board
    void print() const;

    // Method to get a value from the board
    int get(int row, int col) const;

    // Method to set a value in the board
    void set(int row, int col, int value);

    // Method to check for a winner
    int winner() const;

    // Method to output the board as a string
    string to_string() const;
};

#endif // BOARD_H