#include <iostream>
#include <ctime>
#include <algorithm>
#include <cstdlib>
#include "functions.h"
#include <random>
// Print Sudoku Board
void printBoard(int board[9][9]) {
    for (int row = 0; row < 9; row++) {
        for (int col = 0; col < 9; col++) {
            std::cout << board[row][col] << " ";
        }
        std::cout << "\n";
    }
}

// Check if a number can be placed in a cell
bool isValid(int board[9][9], int row, int col, int num) {
    for (int i = 0; i < 9; i++) {
        if (board[row][i] == num || board[i][col] == num) {
            return false;
        }
    }

    int startRow = row - row % 3, startCol = col - col % 3;
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            if (board[startRow + i][startCol + j] == num) {
                return false;
            }
        }
    }
    return true;
}

// Generate a full Sudoku board
bool fillBoard(int board[9][9]) {
    std::random_device rd;
    std::mt19937 g(rd()); 
    for (int row = 0; row < 9; row++) {
        for (int col = 0; col < 9; col++) {
            if (board[row][col] == 0) {
                int numbers[9] = { 1,2,3,4,5,6,7,8,9 };
                std::shuffle(numbers, numbers + 9, g);

                for (int i = 0; i < 9; i++) {
                    int num = numbers[i];

                    if (isValid(board, row, col, num)) {
                        board[row][col] = num;
                        if (fillBoard(board))
                            return true;
                        board[row][col] = 0;
                    }
                }
                return false;
            }
        }
    }
    return true;
}

// Remove numbers to create a puzzle
void prepareBoard(int board[9][9], int quantityOfFreeField) {
    srand(time(0));
    while (quantityOfFreeField > 0) {
        int row = rand() % 9;
        int col = rand() % 9;
        if (board[row][col] != 0) {
            board[row][col] = 0;
            quantityOfFreeField--;
        }
    }
}

// Solve the Sudoku Board using backtracking
bool solveBoard(int board[9][9]) {
    for (int row = 0; row < 9; row++) {
        for (int col = 0; col < 9; col++) {
            if (board[row][col] == 0) {
                for (int num = 1; num <= 9; num++) {
                    if (isValid(board, row, col, num)) {
                        board[row][col] = num;
                        if (solveBoard(board)) {
                            return true;
                        }
                        board[row][col] = 0;
                    }
                }
                return false;
            }
        }
    }
    return true;
}