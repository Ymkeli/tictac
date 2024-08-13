#define BOOST_TEST_MODULE TestBoard
#include <boost/test/included/unit_test.hpp>
#include "board.h"

// Test case for Winner function
BOOST_AUTO_TEST_CASE(WinnerTest_EmptyBoard) {
    // Test if empty board has no winner
    Board board(3,3);
    BOOST_CHECK(board.winner() == 0);
}

BOOST_AUTO_TEST_CASE(WinnerTest_Player1Wins) {
    // Player 1 wins on the first diagonal
    Board board(3,3);
    board.set(0,0,1);
    board.set(1,1,1);
    board.set(2,2,1);
    BOOST_CHECK(board.winner() == 1);
}

BOOST_AUTO_TEST_CASE(WinnerTest_Player2Wins) {
    // Player 2 wins on the first row
    Board board(3,3);
    board.set(0,0,2);
    board.set(0,1,2);
    board.set(0,2,2);
    BOOST_CHECK(board.winner() == 2);
}

BOOST_AUTO_TEST_CASE(WinnerTest_Draw) {
    // Finish the game with a draw
    Board board(3,3);
    board.set(0,0,1);
    board.set(0,1,2);
    board.set(0,2,1);
    board.set(1,0,1);
    board.set(1,1,2);
    board.set(1,2,2);
    board.set(2,0,2);
    board.set(2,1,1);
    board.set(2,2,2);
    BOOST_CHECK(board.winner() == 0);
}

BOOST_AUTO_TEST_CASE(toStringTest) {
    // Print empty board
    Board board(3,3);
    BOOST_CHECK(board.to_string() == "[[0,0,0],[0,0,0],[0,0,0]]");

    // Print board with values
    board.set(0,0,1);
    board.set(1,1,2);
    BOOST_CHECK(board.to_string() == "[[1,0,0],[0,2,0],[0,0,0]]");
}