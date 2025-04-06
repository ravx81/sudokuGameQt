#include "sudokuGameQt.h"
#include <QHeaderView>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QGridLayout>
#include <QLineEdit>
#include <QStyledItemDelegate>
#include <QRegularExpression>
#include <QRegularExpressionValidator>
#include <QMessageBox>
#include <iostream>
#include <cstdlib> 
#include <ctime>

// delegate for single character input in table cells
SingleCharDelegate::SingleCharDelegate(QObject* parent)
    : QStyledItemDelegate(parent)
{
}

QWidget* SingleCharDelegate::createEditor(QWidget* parent,
    const QStyleOptionViewItem& option,
    const QModelIndex& index) const
{
    QLineEdit* editor = new QLineEdit(parent);
    editor->setMaxLength(1);
    QRegularExpression re("[1-9]");
    auto* validator = new QRegularExpressionValidator(re, editor);
    editor->setValidator(validator);
    return editor;
}

sudokuGameQt::sudokuGameQt(QWidget* parent)
    : QMainWindow(parent)
{
    setFixedSize(850, 500);

    hintCounter = 0;
    level = "Easy";

    timer = new QTimer(this);
    startTime = new QTime(QTime::currentTime());

    labelTime = new QLabel("Time: 00:00", this);
    labelTime->setObjectName("labelTime");

    errorLabel = new QLabel("Mistakes: 0/3", this);
    errorLabel->setObjectName("errorLabel");

    centralWidget = new QWidget(this);
    setCentralWidget(centralWidget);

    // create main layout for the central widget
    QHBoxLayout* mainLayout = new QHBoxLayout(centralWidget);
    mainLayout->setContentsMargins(10, 10, 10, 10);
    mainLayout->setSpacing(10);

    // configure table widget for sudoku grid
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

    // set up right side panel with buttons and info labels
    QWidget* rightWidget = new QWidget(this);
    QVBoxLayout* rightLayout = new QVBoxLayout(rightWidget);
    rightLayout->setContentsMargins(0, 0, 0, 0);
    rightLayout->setSpacing(10);

    buttonClear = new QPushButton("", this);
    buttonClear->setObjectName("clearButton");
    buttonUndo = new QPushButton("", this);
    buttonUndo->setObjectName("undoButton");
    buttonNewGame = new QPushButton("New-Game", this);
    buttonNewGame->setObjectName("newGameButton");
    buttonHint = new QPushButton("", this);
    buttonHint->setObjectName("hintButton");

    buttonEasyLevel = new QPushButton("Easy", this);
    buttonEasyLevel->setObjectName("levelButton");
    buttonMediumLevel = new QPushButton("Medium", this);
    buttonMediumLevel->setObjectName("levelButton");
    buttonHardLevel = new QPushButton("Hard", this);
    buttonHardLevel->setObjectName("levelButton");

    connect(buttonEasyLevel, &QPushButton::clicked, this, [this]() {
        level = "Easy";
        QMessageBox::StandardButton reply;
        reply = QMessageBox::question(this, "Confirm", "Do you want to start new game?", QMessageBox::Yes | QMessageBox::No);
        if (reply == QMessageBox::Yes) {
            onDrawBoardClicked(level);
        }
        });
    connect(buttonMediumLevel, &QPushButton::clicked, this, [this]() {
        level = "Medium";
        QMessageBox::StandardButton reply;
        reply = QMessageBox::question(this, "Confirm", "Do you want to start new game?", QMessageBox::Yes | QMessageBox::No);
        if (reply == QMessageBox::Yes) {
            onDrawBoardClicked(level);
        }
        });
    connect(buttonHardLevel, &QPushButton::clicked, this, [this]() {
        level = "Hard";
        QMessageBox::StandardButton reply;
        reply = QMessageBox::question(this, "Confirm", "Do you want to start new game?", QMessageBox::Yes | QMessageBox::No);
        if (reply == QMessageBox::Yes) {
            onDrawBoardClicked(level);
        }
        });
    connect(buttonHint, &QPushButton::clicked, this, &sudokuGameQt::buttonHintClicked);
    connect(buttonNewGame, &QPushButton::clicked, this, [this]() {
        QMessageBox::StandardButton reply;
        reply = QMessageBox::question(this, "Confirm", "Do you want to start new game?", QMessageBox::Yes | QMessageBox::No);
        if (reply == QMessageBox::Yes) {
            onDrawBoardClicked(level);
        }
        });

    // set up layout for info labels (mistakes and time)
    QVBoxLayout* infoLayout = new QVBoxLayout;
    infoLayout->addWidget(errorLabel);
    infoLayout->addWidget(labelTime);
    infoLayout->setAlignment(Qt::AlignLeft);

    // create top row layout with info and central buttons
    QHBoxLayout* topRowLayout = new QHBoxLayout;
    topRowLayout->addLayout(infoLayout);
    topRowLayout->addStretch();

    // create layout for clear, undo, and hint buttons
    QHBoxLayout* centerButtonLayout = new QHBoxLayout;
    centerButtonLayout->addWidget(buttonClear);
    centerButtonLayout->addWidget(buttonUndo);
    centerButtonLayout->addWidget(buttonHint);
    centerButtonLayout->setAlignment(Qt::AlignCenter);

    topRowLayout->addLayout(centerButtonLayout);
    topRowLayout->addStretch();

    rightLayout->addLayout(topRowLayout);

    // create keypad layout for number input
    QGridLayout* keypadLayout = new QGridLayout;
    for (int i = 1; i < 10; i++) {
        QPushButton* button = new QPushButton(QString::number(i), this);
        button->setObjectName("keypadButton");
        connect(button, &QPushButton::clicked, [this, i]() {
            QTableWidgetItem* currentItem = tableWidget->currentItem();
            if (!currentItem) return;
            int row = tableWidget->currentRow();
            int col = tableWidget->currentColumn();
            int oldValue = m_logic.getValue(row, col);
            if (oldValue == 0) {
                int correctVal = m_logic.getSolutionValue(row, col);
                if (correctVal == i) {
                    m_logic.setValue(row, col, i);
                    Move move{ row, col, oldValue, i, Qt::darkGreen };
                    moves.push_back(move);
                    currentItem->setText(QString::number(i));
                    currentItem->setForeground(QBrush(Qt::darkGreen));
                }
                else {
                    Move move{ row, col, oldValue, i, Qt::red };
                    moves.push_back(move);
                    currentItem->setText(QString::number(i));
                    currentItem->setForeground(QBrush(Qt::red));
                    m_logic.setValue(row, col, i);
                    m_logic.incrementMistakes();
                    int mistakes = m_logic.getMistakes();
                    errorLabel->setText(QString("Mistakes: %1/3").arg(mistakes));
                    if (mistakes == 3) {
                        QMessageBox messageBox;
                        messageBox.setText("You lost, 3 mistakes were made!");
                        messageBox.setStandardButtons(QMessageBox::Ok);
                        messageBox.setFixedSize(400, 200);
                        messageBox.exec();
                        m_logic.resetMistakes();
                        onDrawBoardClicked("Easy");
                    }
                }
            }
            });
        int r = (i - 1) / 3;
        int c = (i - 1) % 3;
        keypadLayout->addWidget(button, r, c);
    }

    connect(buttonClear, &QPushButton::clicked, [this]() {
        QTableWidgetItem* currentItem = tableWidget->currentItem();
        if (!currentItem) return;
        int row = tableWidget->currentRow();
        int col = tableWidget->currentColumn();
        int oldValue = m_logic.getValue(row, col);
        if (oldValue != 0) {
            Move move{ row, col, oldValue, 0, currentItem->foreground().color() };
            m_logic.setValue(row, col, 0);
            currentItem->setText("");
            moves.push_back(move);
        }
        });

    connect(buttonUndo, &QPushButton::clicked, [this]() {
        if (moves.empty()) {
            return;
        }
        Move lastMove = moves.back();
        moves.pop_back();
        m_logic.setValue(lastMove.row, lastMove.col, lastMove.oldValue);
        QTableWidgetItem* item = tableWidget->item(lastMove.row, lastMove.col);
        if (lastMove.oldValue == 0) {
            item->setText("");
        }
        else {
            item->setText(QString::number(lastMove.oldValue));
        }
        });

    keypadLayout->setContentsMargins(95, 0, 0, 0);
    rightLayout->addLayout(keypadLayout);

    // create layout for difficulty level selection buttons
    QHBoxLayout* difficultyLayout = new QHBoxLayout;
    difficultyLayout->addWidget(buttonEasyLevel);
    difficultyLayout->addWidget(buttonMediumLevel);
    difficultyLayout->addWidget(buttonHardLevel);
    difficultyLayout->setContentsMargins(95, 0, 0, 0);
    rightLayout->addLayout(difficultyLayout);

    // create layout for new game button
    QHBoxLayout* newGameLayout = new QHBoxLayout;
    newGameLayout->addSpacing(100);
    newGameLayout->addWidget(buttonNewGame);
    newGameLayout->setAlignment(buttonNewGame, Qt::AlignCenter);
    rightLayout->addLayout(newGameLayout);

    connect(timer, &QTimer::timeout, this, &sudokuGameQt::updateTimer);
    timer->start(1000);
    rightLayout->addStretch();

    mainLayout->addWidget(rightWidget, 0, Qt::AlignTop);

    // initialize game board with default difficulty
    onDrawBoardClicked("Easy");
}

sudokuGameQt::~sudokuGameQt()
{
}

void sudokuGameQt::onDrawBoardClicked(std::string difficulty)
{
    moves.clear();
    m_logic.generateBoard(difficulty);
    hintCounter = 0;
    for (int row = 0; row < 9; row++) {
        for (int col = 0; col < 9; col++) {
            int val = m_logic.getValue(row, col);
            QTableWidgetItem* item = tableWidget->item(row, col);
            if (val == 0) {
                item->setText("");
            }
            else {
                item->setText(QString::number(val));
                item->setForeground(Qt::black);
            }
        }
    }
    m_logic.resetMistakes();
    errorLabel->setText("Mistakes: 0/3");
    *startTime = QTime::currentTime();
}

void sudokuGameQt::onSolveBoardClicked()
{
    bool ok = m_logic.solveCurrentBoard();
    if (!ok) {
        std::cout << "Problems: board is unsolvable.\n";
    }
    for (int row = 0; row < 9; row++) {
        for (int col = 0; col < 9; col++) {
            int val = m_logic.getValue(row, col);
            tableWidget->item(row, col)->setText(QString::number(val));
        }
    }
}

void sudokuGameQt::updateTimer()
{
    int elapsed = startTime->msecsTo(QTime::currentTime());
    QTime displayTime = QTime::fromMSecsSinceStartOfDay(elapsed);
    labelTime->setText("Time: " + displayTime.toString("mm:ss"));
}

void sudokuGameQt::buttonHintClicked()
{
    if (hintCounter >= 1) {
        QMessageBox::information(this, "No more hints", "You used your hint already!");
        return;
    }
    std::vector<std::pair<int, int>> emptyCells;
    for (int row = 0; row < 9; row++) {
        for (int col = 0; col < 9; col++) {
            if (m_logic.getValue(row, col) == 0) {
                emptyCells.push_back({ row, col });
            }
        }
    }
    if (emptyCells.empty()) {
        QMessageBox::information(this, "No empty cells", "Board is already full!");
        return;
    }
    int randomIndex = rand() % emptyCells.size();
    int row = emptyCells[randomIndex].first;
    int col = emptyCells[randomIndex].second;
    int solVal = m_logic.getSolutionValue(row, col);
    m_logic.setValue(row, col, solVal);
    QTableWidgetItem* item = tableWidget->item(row, col);
    item->setText(QString::number(solVal));
    item->setForeground(QBrush(Qt::blue));
    hintCounter++;
}
