#include "sudokuGameQt.h"
#include "functions.h"
#include <QTableWidget>
#include <QHeaderView>
#include <QHBoxLayout>
#include <QTableWidgetItem>
#include <QStyledItemDelegate>
#include <QLineEdit>
#include <QRegularExpression>
#include <QRegularExpressionValidator>


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
        // Create a line edit with a single-character limit
        QLineEdit* editor = new QLineEdit(parent);
        editor->setMaxLength(1);

        // Optionally, if you want to allow only digits 1-9, use a validator:
        QRegularExpression re("[1-9]");  // digits 1 through 9
        QValidator* validator = new QRegularExpressionValidator(re, editor);
        editor->setValidator(validator);

        return editor;
    }
};


sudokuGameQt::sudokuGameQt(QWidget* parent)
    : QMainWindow(parent)
{
    setFixedSize(900, 650);

    setStyleSheet(
        "QMainWindow {"
        "    background-image: url(background.png);"
        "    background-repeat: no-repeat;"
        "    background-position: top-center;"
        "    background-size: cover;"
        "}"
    );

    centralWidget = new QWidget(this);
    centralWidget->setAttribute(Qt::WA_TranslucentBackground);
    setCentralWidget(centralWidget);

    layout = new QVBoxLayout(centralWidget);
    layout->setContentsMargins(0, 0, 0, 0);
    layout->setSpacing(0);

    tableWidget = new QTableWidget(9, 9, this);
    tableWidget->setItemDelegate(new SingleCharDelegate(this));



    tableWidget->horizontalHeader()->setVisible(false);
    tableWidget->verticalHeader()->setVisible(false);

    tableWidget->horizontalHeader()->setSectionResizeMode(QHeaderView::Fixed);
    tableWidget->verticalHeader()->setSectionResizeMode(QHeaderView::Fixed);

    int cellSize = 47;
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

    for (int row = 0; row < 9; ++row) {
        for (int col = 0; col < 9; ++col) {
            QTableWidgetItem* item = new QTableWidgetItem("");
            item->setTextAlignment(Qt::AlignCenter);
            tableWidget->setItem(row, col, item);
            
        }
    }
    int totalWidth = (cellSize * 9) + tableWidget->frameWidth() * 2;
    int totalHeight = (cellSize * 9) + tableWidget->frameWidth() * 2;
    tableWidget->setFixedSize(totalWidth, totalHeight);

    QHBoxLayout* centerLayout = new QHBoxLayout;
    centerLayout->setContentsMargins(0, 0, 0, 0);
    centerLayout->addStretch();         
    centerLayout->addWidget(tableWidget);
    centerLayout->addStretch(); 

   
    layout->addLayout(centerLayout);

   
    button1 = new QPushButton("Draw board", this);
    button2 = new QPushButton("Solve board", this);

    
    buttonLayout = new QHBoxLayout;
    buttonLayout->addWidget(button1);
    buttonLayout->addWidget(button2);

   
    layout->addLayout(buttonLayout);

   
    connect(button1, &QPushButton::clicked, this, &sudokuGameQt::onDrawBoardClicked);
    connect(button2, &QPushButton::clicked, this, &sudokuGameQt::onSolveBoardClicked);



    QGridLayout* keypadLayout = new QGridLayout;

    for (int i = 1; i < 10; i++) {
        QPushButton* button = new QPushButton(QString::number(i), this);
        button->setFixedSize(50, 50);

        connect(button, &QPushButton::clicked, [this, i]() {
            QTableWidgetItem* currentItem = tableWidget->currentItem();
            if (currentItem) {
                currentItem->setText(QString::number(i));
            }
            });
        int row = (i - 1) / 3;
        int col = (i - 1) % 3;
        keypadLayout->addWidget(button, row, col);       
    }
 
    QPushButton* clearButton = new QPushButton("Clear", this);
    connect(clearButton, &QPushButton::clicked, [this]() {
        QTableWidgetItem* currentItem = tableWidget->currentItem();
        if (currentItem) {
            currentItem->setText("");
        }
    });
    keypadLayout->addWidget(clearButton, 3, 0, 1, 3);

    layout->addLayout(keypadLayout);

}

sudokuGameQt::~sudokuGameQt()
{
    
}

void sudokuGameQt::onDrawBoardClicked()
{
    int board[9][9] = { 0 };

    fillBoard(board);
    prepareBoard(board, 40);

    for (int row = 0; row < 9; row++) {
        for (int col = 0; col < 9; col++) {
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
