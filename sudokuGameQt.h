#pragma once
#include <QMainWindow>
#include <QTableWidget>
#include <QPushButton>
#include <QLabel>
#include <QTimer>
#include <QTime>
#include <vector>
#include <QStyledItemDelegate>
#include "SudokuLogic.h"

class SingleCharDelegate : public QStyledItemDelegate
{
    Q_OBJECT
public:
    SingleCharDelegate(QObject* parent = nullptr);

    QWidget* createEditor(QWidget* parent,
        const QStyleOptionViewItem& option,
        const QModelIndex& index) const override;
};

struct Move {
    int row, col, oldValue, newValue;
    QColor color;
};

class sudokuGameQt : public QMainWindow
{
    Q_OBJECT
public:
    sudokuGameQt(QWidget* parent = nullptr);
    ~sudokuGameQt();

private slots:
    void onDrawBoardClicked(std::string level);
    void onSolveBoardClicked();
    void updateTimer();
    void buttonHintClicked();

private:
    QWidget* centralWidget;
    QTableWidget* tableWidget;
    QPushButton* buttonUndo;
    QPushButton* buttonClear;
    QPushButton* buttonHint;
    QPushButton* buttonNewGame;
    QPushButton* buttonEasyLevel;
    QPushButton* buttonMediumLevel;
    QPushButton* buttonHardLevel;

    QLabel* labelTime;
    QLabel* errorLabel;

    QTimer* timer;
    QTime* startTime;

    SudokuLogic m_logic;

    std::vector<Move> moves;

    std::string level;
    int hintCounter;
};
