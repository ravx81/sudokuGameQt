#ifndef FUNCTIONS_H
#define FUNCTIONS_H

void printBoard(int board[9][9]);
bool isValid(int board[9][9], int row, int col, int num);
bool fillBoard(int board[9][9]);
void prepareBoard(int board[9][9], int quantityOfFreeField);
bool solveBoard(int board[9][9]);

#endif
