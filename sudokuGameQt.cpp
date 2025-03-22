#include <QApplication>
#include <QMainWindow>
#include <QTableWidget>
#include <QHeaderView>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QGridLayout>
#include <QPushButton>
#include <QTableWidgetItem>
#include <QStyledItemDelegate>
#include <QLineEdit>
#include <QRegularExpression>
#include <QRegularExpressionValidator>
#include <QWidget>
#include <qlabel.h>
#include <iostream>

#include "functions.h" 

class SingleCharDelegate : public QStyledItemDelegate
{
public:
    SingleCharDelegate(QObject* parent = nullptr)
        : QStyledItemDelegate(parent)
    {
    }

    QWidget* createEditor(QWidget* parent,
        const QStyleOptionViewItem& option,
        const QModelIndex& index) const override
    {
        QLineEdit* editor = new QLineEdit(parent);
        editor->setMaxLength(1);
        QRegularExpression re("[1-9]");
        QValidator* validator = new QRegularExpressionValidator(re, editor);
        editor->setValidator(validator);
        return editor;
    }
};


class sudokuGameQt : public QMainWindow
{
    Q_OBJECT
public:
    sudokuGameQt(QWidget* parent = nullptr);
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
    int solutionBoard[9][9];
    int board[9][9];
    int error;
};

sudokuGameQt::sudokuGameQt(QWidget* parent)
    : QMainWindow(parent)
{

    setFixedSize(850, 600);
    setStyleSheet(
        "QMainWindow {"
        "    background-image: url(background.png);"
        "    background-repeat: no-repeat;"
        "    background-position: top left;"
        "    background-size: cover;"
        "}"
    );

    
    centralWidget = new QWidget(this);
    centralWidget->setAttribute(Qt::WA_TranslucentBackground);
    setCentralWidget(centralWidget);

   
    QHBoxLayout* mainLayout = new QHBoxLayout(centralWidget);
    mainLayout->setContentsMargins(10, 10, 10, 10);
    mainLayout->setSpacing(10);

    
    tableWidget = new QTableWidget(9, 9, this);
    tableWidget->setItemDelegate(new SingleCharDelegate(this));
    tableWidget->horizontalHeader()->setVisible(false);
    tableWidget->verticalHeader()->setVisible(false);
    tableWidget->horizontalHeader()->setSectionResizeMode(QHeaderView::Fixed);
    tableWidget->verticalHeader()->setSectionResizeMode(QHeaderView::Fixed);

    int cellSize = 48;
    tableWidget->horizontalHeader()->setDefaultSectionSize(cellSize);
    tableWidget->verticalHeader()->setDefaultSectionSize(cellSize);
    tableWidget->setFrameStyle(QFrame::NoFrame);
    tableWidget->setShowGrid(false);
    tableWidget->viewport()->setAutoFillBackground(false);
    tableWidget->setStyleSheet(
        "QTableWidget {"
        "    background-color: rgba(255, 255, 255, 0);"
        "}"
        "QTableView {"
        "    background-color: rgba(255, 255, 255, 0);"
        "}"
    );

    QFont font;
    font.setBold(true);
    for (int row = 0; row < 9; ++row) {
        for (int col = 0; col < 9; ++col) {
            QTableWidgetItem* item = new QTableWidgetItem("");
            item->setTextAlignment(Qt::AlignCenter);
            item->setFont(font);
            tableWidget->setItem(row, col, item);
        }
    }
    int totalWidth = (cellSize * 9) + tableWidget->frameWidth() * 2;
    int totalHeight = (cellSize * 9) + tableWidget->frameWidth() * 2;
    tableWidget->setFixedSize(totalWidth, totalHeight);

    
    mainLayout->addWidget(tableWidget, 0, Qt::AlignLeft | Qt::AlignTop);

   
    QWidget* rightWidget = new QWidget(this);
    QVBoxLayout* rightLayout = new QVBoxLayout(rightWidget);
    rightLayout->setContentsMargins(0, 0, 0, 0);
    rightLayout->setSpacing(10);

    
    buttonClear = new QPushButton("", this);
    buttonSolve = new QPushButton("Solve", this);
    buttonUndo = new QPushButton("", this);
    buttonNewGame = new QPushButton("New-Game", this);
    buttonHint = new QPushButton("", this);

    buttonClear->setFixedSize(60, 70);
    buttonSolve->setFixedSize(60, 70);
    buttonUndo->setFixedSize(60, 70);
    buttonNewGame->setFixedSize(60, 70);
    buttonHint->setFixedSize(60, 70);

    buttonClear->setStyleSheet(
        "QPushButton { "
        "    border-image: url(:/images/rubber.png) 0 0 0 0 stretch stretch;"
        "}"
    );

    buttonUndo->setStyleSheet(
        "QPushButton { "
        "    border-image: url(:/images/undo.png) 0 0 0 0 stretch stretch;"
        "}"
    );

    buttonHint->setStyleSheet(
        "QPushButton { "
        "    border-image: url(:/images/hint.png) 0 0 0 0 stretch stretch;"
        "}"
    );

    connect(buttonNewGame, &QPushButton::clicked, this, &sudokuGameQt::onDrawBoardClicked);
    connect(buttonSolve, &QPushButton::clicked, this, &sudokuGameQt::onSolveBoardClicked);


    QHBoxLayout* buttonLayout = new QHBoxLayout;
    buttonLayout->addWidget(buttonClear);
    buttonLayout->addWidget(buttonSolve);
    buttonLayout->addWidget(buttonUndo);
    buttonLayout->addWidget(buttonNewGame);
    buttonLayout->addWidget(buttonHint);


    rightLayout->addLayout(buttonLayout);

    
    QGridLayout* keypadLayout = new QGridLayout;

    QLabel* errorLabel = new QLabel("0", this);
    errorLabel->setStyleSheet("font-size: 18pt; color: red;");

    for (int i = 1; i < 10; i++) {
        QPushButton* button = new QPushButton(QString::number(i), this);
        button->setFixedSize(50, 50);
        button->setStyleSheet(
            "QPushButton { "
            "   background-color: green;"
            "}"
        );
    
        connect(button, &QPushButton::clicked, [this, i, errorLabel]() {
            QTableWidgetItem* currentItem = tableWidget->currentItem();
            if (currentItem) {
                int row = tableWidget->currentRow();
                int col = tableWidget->currentColumn();

                if (solutionBoard[row][col] == i) {
                    board[row][col] = i;
                    currentItem->setForeground(QBrush(Qt::darkGreen));
                    currentItem->setText(QString::number(i));
                }
                else {
                    currentItem->setForeground(QBrush(Qt::red));
                    currentItem->setText(QString::number(i));
                    error++;
                    errorLabel->setText(QString::number(error));
                }
            }
            });
        int row = (i - 1) / 3;
        int col = (i - 1) % 3;
        keypadLayout->addWidget(button, row, col);
    }
    connect(buttonClear, &QPushButton::clicked, [this, errorLabel]() {
        QTableWidgetItem* currentItem = tableWidget->currentItem();
        if (currentItem) {
            QBrush brush = currentItem->foreground();
            QColor color = brush.color();
            if (color == Qt::red) {
                error--;
                errorLabel->setText(QString::number(error));
            }
            currentItem->setText("");
        }
        });
    rightLayout->addLayout(keypadLayout);
    rightLayout->addWidget(errorLabel);
    rightLayout->addStretch();
    
    mainLayout->addWidget(rightWidget, 0, Qt::AlignTop | Qt::AlignRight);

    onDrawBoardClicked();
}

sudokuGameQt::~sudokuGameQt()
{
}

void sudokuGameQt::onDrawBoardClicked()
{
    error = 0;
    int tempBoard[9][9] = { 0 };

    fillBoard(tempBoard);
    memcpy(solutionBoard, tempBoard, sizeof(tempBoard));

    prepareBoard(tempBoard, 40);

    for (int row = 0; row < 9; row++) {
        for (int col = 0; col < 9; col++) {
            board[row][col] = tempBoard[row][col];
            if (board[row][col] == 0) {
                tableWidget->item(row, col)->setText("");
            }
            else {
                tableWidget->item(row, col)->setText(QString::number(board[row][col]));
            }
        }
    }
}

void sudokuGameQt::onSolveBoardClicked()
{
    int board[9][9];
    
    for (int row = 0; row < 9; row++) {
        for (int col = 0; col < 9; col++) {
            QString cellText = tableWidget->item(row, col)->text();
            if (!cellText.isEmpty()) {
                board[row][col] = cellText.toInt();
            }
            else {
                board[row][col] = 0;
            }
        }
    }
    if (solveBoard(board)) {
        for (int row = 0; row < 9; row++) {
            for (int col = 0; col < 9; col++) {
                tableWidget->item(row, col)->setText(QString::number(board[row][col]));
            }
        }
    }
    else {
        std::cout << "Problems";
    }
}
