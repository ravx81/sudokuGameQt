#include "SudokuLogic.h"
#include <algorithm>
#include <cstdlib>
#include <iostream>

SudokuLogic::SudokuLogic()
{
    for (auto& row : m_board) {
        row.fill(0);
    }
    for (auto& row : m_solution) {
        row.fill(0);
    }
    m_mistakes = 0;
}

void SudokuLogic::generateBoard(const std::string& level)
{
    for (auto& row : m_solution) {
        row.fill(0);
    }
    fillBoard(m_solution);
    m_board = m_solution;
    int emptyFields;

    if (level == "Easy") emptyFields = 30;
    else if (level == "Medium") emptyFields = 40;
    else if (level == "Hard") emptyFields = 50;
    prepareBoard(m_board, emptyFields);
    resetMistakes();
}

bool SudokuLogic::fillBoard(std::array<std::array<int, 9>, 9>& board)
{
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
                        if (fillBoard(board)) {
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

void SudokuLogic::prepareBoard(std::array<std::array<int, 9>, 9>& board, int quantityOfFreeField)
{
    srand((unsigned)time(nullptr));
    while (quantityOfFreeField > 0) {
        int row = rand() % 9;
        int col = rand() % 9;
        if (board[row][col] != 0) {
            board[row][col] = 0;
            quantityOfFreeField--;
        }
    }
}

bool SudokuLogic::isValid(const std::array<std::array<int, 9>, 9>& board, int row, int col, int num) const
{
    for (int i = 0; i < 9; i++) {
        if (board[row][i] == num) return false;
        if (board[i][col] == num) return false;
    }

    int startRow = row - row % 3;
    int startCol = col - col % 3;
    for (int r = 0; r < 3; r++) {
        for (int c = 0; c < 3; c++) {
            if (board[startRow + r][startCol + c] == num) {
                return false;
            }
        }
    }
    return true;
}

bool SudokuLogic::solveBoard(std::array<std::array<int, 9>, 9>& brd)
{
    for (int row = 0; row < 9; row++) {
        for (int col = 0; col < 9; col++) {
            if (brd[row][col] == 0) {
                for (int num = 1; num <= 9; num++) {
                    if (isValid(brd, row, col, num)) {
                        brd[row][col] = num;
                        if (solveBoard(brd)) {
                            return true;
                        }
                        brd[row][col] = 0;
                    }
                }
                return false;
            }
        }
    }
    return true;
}

bool SudokuLogic::solveCurrentBoard()
{
    return solveBoard(m_board);
}

int SudokuLogic::getValue(int row, int col) const
{
    return m_board[row][col];
}

void SudokuLogic::setValue(int row, int col, int val)
{
    m_board[row][col] = val;
}

int SudokuLogic::getSolutionValue(int row, int col) const
{
    return m_solution[row][col];
}
