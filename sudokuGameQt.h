#pragma once

#include <QtWidgets/QMainWindow>
#include <QPushButton>
#include <QLabel>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLineEdit>
#include <QTableWidgetItem>
#include <QTableWidget>
#include <iostream>
class sudokuGameQt : public QMainWindow
{
    Q_OBJECT

public:
    explicit sudokuGameQt(QWidget* parent = nullptr);
    ~sudokuGameQt();

private slots:
    void onDrawBoardClicked();
    void onSolveBoardClicked();


private:
    QWidget* centralWidget;
    QTableWidget* tableWidget;
    QPushButton* buttonSolve;
    QPushButton* buttonUndo;
    QPushButton* buttonClear;
    QPushButton* buttonHint;
    QPushButton* buttonNewGame;
    int board[9][9];
    int solutionBoard[9][9];
};
