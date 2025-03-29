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
#include <qtimer.h>
#include <qtime>
#include <qlabel.h>
#include <iostream>
#include <vector>
#include "functions.h"
#include <qmessagebox.h>

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

    int solutionBoard[9][9];
    int board[9][9];
    int error;
    std::vector<Move> moves;
    QTimer* timer;
    QTime* startTime;
    QLabel* labelTime;
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

    timer = new QTimer(this);
    startTime = new QTime(QTime::currentTime());

    labelTime = new QLabel("0 seconds", this);
    labelTime->setStyleSheet("font-size: 20pt; font-weight: bold;");

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
    buttonUndo = new QPushButton("", this);
    buttonNewGame = new QPushButton("New-Game", this);
    buttonHint = new QPushButton("", this);

    buttonEasyLevel = new QPushButton("Easy", this);
    buttonMediumLevel = new QPushButton("Medium", this);
    buttonHardLevel = new QPushButton("Hard", this);

    buttonEasyLevel->setStyleSheet(
        "QPushButton { "
        "    background-color: #ADD8E6;"
        "    color: #333;"
        "    font-weight: bold;"
        "    border: none;"
        "    border-radius: 5px;"
        "}"
        "QPushButton:hover { "
        "    background-color: #B9DAEB;"
        "}"
    );

    buttonMediumLevel->setStyleSheet(
        "QPushButton { "
        "    background-color: #ADD8E6;"
        "    color: #333;"
        "    font-weight: bold;"
        "    border: none;"
        "    border-radius: 5px;"
        "}"
        "QPushButton:hover { "
        "    background-color: #B9DAEB;"
        "}"
    );

    buttonHardLevel->setStyleSheet(
        "QPushButton { "
        "    background-color: #ADD8E6;"
        "    color: #333;"
        "    font-weight: bold;"
        "    border: none;"
        "    border-radius: 5px;"
        "}"
        "QPushButton:hover { "
        "    background-color: #B9DAEB;"
        "}"
    );

  
    buttonClear->setFixedSize(60, 70);
    buttonUndo->setFixedSize(60, 70);
    buttonNewGame->setFixedSize(60, 70);
    buttonHint->setFixedSize(60, 70);

    buttonEasyLevel->setFixedSize(75,35);
    buttonMediumLevel->setFixedSize(75, 35);
    buttonHardLevel->setFixedSize(75, 35);


    

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

    connect(buttonEasyLevel, &QPushButton::clicked, this, [this]() {
        onDrawBoardClicked("Easy");
        });
    
    connect(buttonMediumLevel, &QPushButton::clicked, this, [this]() {
        onDrawBoardClicked("Medium");
        });

    connect(buttonHardLevel, &QPushButton::clicked, this, [this]() {
        onDrawBoardClicked("Hard");
        });


    QHBoxLayout* buttonLayout = new QHBoxLayout;
    buttonLayout->addWidget(buttonClear);
    buttonLayout->addWidget(buttonUndo);
    buttonLayout->addWidget(buttonNewGame);
    buttonLayout->addWidget(buttonHint);

    
    rightLayout->addLayout(buttonLayout);

    
    QGridLayout* keypadLayout = new QGridLayout;

    QLabel* errorLabel = new QLabel("Mistakes: 0/3", this);
    errorLabel->setStyleSheet("font-size: 18pt; color: red; font-weight: bold;");
    
    for (int i = 1; i < 10; i++) {
        QPushButton* button = new QPushButton(QString::number(i), this);
        button->setFixedSize(50, 50);
        button->setStyleSheet(
            "QPushButton { "
            "    background-color: #ADD8E6;" 
            "    color: #333;"             
            "    font-weight: bold;"
            "    border: none;"
            "    border-radius: 5px;"      
            "}"
            "QPushButton:hover { "
            "    background-color: #B9DAEB;" 
            "}"
        );
    
        connect(button, &QPushButton::clicked, [this, i, errorLabel]() {
            QTableWidgetItem* currentItem = tableWidget->currentItem();
            if (currentItem) {
                int row = tableWidget->currentRow();
                int col = tableWidget->currentColumn();
                int oldValue = board[row][col];
                              
                if (solutionBoard[row][col] == i) {
                    board[row][col] = i;
                    QColor color = Qt::darkGreen;
                    Move move{ row, col,oldValue, i, color };
                    moves.push_back(move);
                    currentItem->setForeground(QBrush(color));
                    currentItem->setText(QString::number(i));
                }
                else {
                    QColor color = Qt::red;
                    Move move{ row, col,oldValue, i, color };
                    moves.push_back(move);
                    currentItem->setForeground(QBrush(color));
                    currentItem->setText(QString::number(i));
                    error++;
                    errorLabel->setText("Mistakes: " + QString::number(error) + "/3");
                    if (error == 3) {
                        QMessageBox::information(this, "Game Over", "You lost");
                        error = 0;
                        //errorLabel->setText("Mistakes: " + QString::number(error) + "/3");
                    }
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
            int row = tableWidget->currentRow();
            int col = tableWidget->currentColumn();
            int oldValue = board[row][col];
            QBrush brush = currentItem->foreground();
            QColor color = brush.color();
            Move move{ row, col,oldValue, 0, color };
    
            if (color == Qt::red) {
                error--;
                board[row][col] = 0;
                currentItem->setText("");
                errorLabel->setText("Mistakes: " + QString::number(error) + "/3");
                moves.push_back(move);
            }
            else if (color == Qt::darkGreen) {
                board[row][col] = 0;
                currentItem->setText("");
                moves.push_back(move);

            }
        }
        });
    connect(buttonUndo, &QPushButton::clicked, [this, errorLabel]() {
        if (moves.empty()) {
            return;
        }

        Move lastMove = moves.back();
        moves.pop_back();

        board[lastMove.row][lastMove.col] = lastMove.oldValue;
        if (lastMove.color == Qt::red) {
            error--;
            errorLabel->setText(QString::number(error));
        }
  

        if (lastMove.oldValue == 0){
            tableWidget->item(lastMove.row, lastMove.col)->setText("");
        }
        else {
            tableWidget->item(lastMove.row, lastMove.col)->setText(QString::number(lastMove.oldValue));
        }
    
        });
    rightLayout->addLayout(keypadLayout);


    QHBoxLayout* infoLayout = new QHBoxLayout;
    infoLayout->addWidget(errorLabel);
    infoLayout->addSpacing(10);  // optional spacing
    infoLayout->addWidget(labelTime);

    rightLayout->addLayout(infoLayout);


    QHBoxLayout* difficultyLayout = new QHBoxLayout;
    difficultyLayout->addWidget(buttonEasyLevel);
    difficultyLayout->addWidget(buttonMediumLevel);
    difficultyLayout->addWidget(buttonHardLevel);

    rightLayout->addLayout(difficultyLayout);

    connect(timer, &QTimer::timeout, this, &sudokuGameQt::updateTimer);
    timer->start(1000);
    rightLayout->addStretch();
    
    mainLayout->addWidget(rightWidget, 0, Qt::AlignTop | Qt::AlignRight);

    onDrawBoardClicked("Easy");
}

sudokuGameQt::~sudokuGameQt()
{
}

void sudokuGameQt::onDrawBoardClicked(std::string level)
{

    error = 0;
    int tempBoard[9][9] = { 0 };

    fillBoard(tempBoard);
    memcpy(solutionBoard, tempBoard, sizeof(tempBoard));
   
    if (level == "Easy") {
        prepareBoard(tempBoard, 30);

    }
    else if (level == "Medium") {
        prepareBoard(tempBoard, 40);
    }
    else if (level == "Hard") {
        prepareBoard(tempBoard, 50);
    }

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
void sudokuGameQt::updateTimer() {
    int elapsed = startTime->msecsTo(QTime::currentTime());
    QTime displayTime = QTime::fromMSecsSinceStartOfDay(elapsed);
    labelTime->setText("Time: " + displayTime.toString("mm:ss"));
}

