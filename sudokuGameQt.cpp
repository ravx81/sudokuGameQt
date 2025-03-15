#include "sudokuGameQt.h"
#include <QTableWidget>
#include <QHeaderView>
#include <QHBoxLayout>
#include <QTableWidgetItem>

sudokuGameQt::sudokuGameQt(QWidget* parent)
    : QMainWindow(parent)
{
    setFixedSize(800, 600);

    setStyleSheet(
        "QMainWindow {"
        "    background-image: url(background.png);"
        "    background-repeat: no-repeat;"
        "    background-position: center;"
        "    background-size: cover;"
        "}"
    );

    centralWidget = new QWidget(this);
    centralWidget->setAttribute(Qt::WA_TranslucentBackground);
    setCentralWidget(centralWidget);

    layout = new QVBoxLayout(centralWidget);
    layout->setContentsMargins(0, 15, 0, 0);
    layout->setSpacing(0);

    tableWidget = new QTableWidget(9, 9, this);

    tableWidget->horizontalHeader()->setVisible(false);
    tableWidget->verticalHeader()->setVisible(false);

    tableWidget->horizontalHeader()->setSectionResizeMode(QHeaderView::Fixed);
    tableWidget->verticalHeader()->setSectionResizeMode(QHeaderView::Fixed);

    int cellSize = 49;
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

   
    layout->addStretch();
    layout->addLayout(centerLayout);
    layout->addStretch();

   
    button1 = new QPushButton("Draw board", this);
    button2 = new QPushButton("Solve board", this);

    
    buttonLayout = new QHBoxLayout;
    buttonLayout->addWidget(button1);
    buttonLayout->addWidget(button2);

   
    layout->addLayout(buttonLayout);

   
    connect(button1, &QPushButton::clicked, this, &sudokuGameQt::onDrawBoardClicked);
    connect(button2, &QPushButton::clicked, this, &sudokuGameQt::onSolveBoardClicked);
}

sudokuGameQt::~sudokuGameQt()
{
    
}

void sudokuGameQt::onDrawBoardClicked()
{
    
}

void sudokuGameQt::onSolveBoardClicked()
{
   
}
