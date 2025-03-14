#include "sudokuGameQt.h"

sudokuGameQt::sudokuGameQt(QWidget* parent)
    : QMainWindow(parent)
{
   
    setFixedSize(800, 600);
    setStyleSheet("QMainWindow {"
                  "background-image: url(background.png);"
                  "background-repeat: no-repeat;"
                  "background-position: center;"
                  "background-size: cover;"
                  "}");
  
    centralWidget = new QWidget(this);
    centralWidget->setAttribute(Qt::WA_TranslucentBackground);
    setCentralWidget(centralWidget);

    layout = new QVBoxLayout(centralWidget);
    layout->setContentsMargins(20, 20, 20, 20);
    layout->setSpacing(10);
    
    //labelOutput = new QLabel(this);
    //labelOutput->setStyleSheet("font-size: 18px; color: black;"); 
    //layout->addWidget(labelOutput);

    layout->addStretch();
   
    button1 = new QPushButton("Draw board", this);
    button2 = new QPushButton("Solve board", this);

    QLineEdit* cell1 = new QLineEdit;
    QLineEdit* cell2 = new QLineEdit;

    for (int row = 0; row < 81; row++) {
        for (int col = 0; col < 81; col++) {
            QLineEdit* cell = new QLineEdit;

            cell->setFixedSize(40, 40);
            cell->setAlignment(Qt::AlignCenter);
            layout->addWidget(cell);
        }
    }

    buttonLayout = new QHBoxLayout;
    buttonLayout->addWidget(button1);
    buttonLayout->addWidget(button2);
    
    layout->addLayout(buttonLayout);

    connect(button1, &QPushButton::clicked, this, &sudokuGameQt::onDrawBoardClicked);
    connect(button2, &QPushButton::clicked, this, &sudokuGameQt::onSolveBoardClicked);
}

sudokuGameQt::~sudokuGameQt()
{
    // Qt automatically deletes child widgets.
}

void sudokuGameQt::onDrawBoardClicked()
{
    
    //labelOutput->setText("Drawing board...");
}

void sudokuGameQt::onSolveBoardClicked()
{
   
    //labelOutput->setText("Solving board...");
}
