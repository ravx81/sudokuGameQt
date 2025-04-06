#pragma once
#include <array>
#include <random>
#include <ctime>
#include <vector>

class SudokuLogic
{
public:
    SudokuLogic();

    void generateBoard(const std::string& level);

    bool solveCurrentBoard();

    int  getValue(int row, int col) const;
    void setValue(int row, int col, int val);

    int  getSolutionValue(int row, int col) const;

    void resetMistakes() { m_mistakes = 0; }
    int  getMistakes() const { return m_mistakes; }
    void incrementMistakes() { m_mistakes++; }
    void decrementMistakes() {
        if (m_mistakes > 0) {
            m_mistakes--;
        }
    }

private:
    std::array<std::array<int, 9>, 9> m_board;    
    std::array<std::array<int, 9>, 9> m_solution; 
    int m_mistakes = 0;

    bool fillBoard(std::array<std::array<int, 9>, 9>& brd);
    void prepareBoard(std::array<std::array<int, 9>, 9>& brd, int quantityOfFreeField);

    bool solveBoard(std::array<std::array<int, 9>, 9>& brd);

    bool isValid(const std::array<std::array<int, 9>, 9>& brd, int row, int col, int num) const;
};
