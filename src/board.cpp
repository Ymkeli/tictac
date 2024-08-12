#include "board.h"

// Constructor
Board::Board(int r, int c) {
    rows = r;
    cols = c;
    grid.resize(rows);
    for(int x = 0; x < rows; x++)
    {
        grid[x].resize(cols);
        for(int y = 0; y < cols; y++)
            grid[x][y] = 0;
    }
}

// Translate the player number in the grid into a player symbol for printing
static string player_tag (int x) {
    switch (x) {
        case 0:
            return ".";
            break;
        case 1:
            return "O";
            break;
        case 2: 
            return "X";
            break;

    }
    return "";
}

// Print board
void Board::print() const {
    for(int x = 0; x < rows; x++) {
        for(int y = 0; y < cols; y++) {
            cout << player_tag(grid[x][y]) << " ";
        }
        cout << endl;
    }
}

// Gets value at position (row,col) from the board
int Board::get(int row, int col) const {
    return grid[row][col];
}

// Set value at position (row,col) at the board
void Board::set(int row, int col, int val) {
    grid[row][col] = val;
}

// Checks if there is a winner for the current board. Returns the player if yes (1 or 2), and 0 otherwise.
int Board::winner() const {
    // Check rows
    for(int r = 0; r < rows; r++) {
        if ((grid[r][0] == grid[r][1]) && (grid[r][0] == grid[r][2]) && grid[r][0] != 0)
            return grid[r][0];
    }

    // Check columns
    for(int c = 0; c < cols; c++) {
        if ((grid[0][c] == grid[1][c]) && (grid[0][c] == grid[2][c]) && grid[0][c] != 0)
            return grid[0][c];
    }

    // Check diagonals
    if((grid[0][0] == grid[1][1]) && (grid[0][0] == grid[2][2]) && grid[0][0] != 0)
        return grid[0][0];
    if((grid[0][2] == grid[1][1]) && (grid[0][2] == grid[2][0]) && grid[0][2] != 0)
        return grid[0][2];

    return 0; //no winner    
}

string Board::to_string() const {
    ostringstream oss;
    oss << "[";

    for (int i = 0; i < rows; ++i) {
        oss << "[";
        for (int j = 0; j < cols; ++j) {
            oss << grid[i][j];
            if (j < cols - 1) {
                oss << ",";
            }
        }
        oss << "]";
        if (i < rows - 1) {
            oss << ",";
        }
    }
    oss << "]";
    return oss.str();
}