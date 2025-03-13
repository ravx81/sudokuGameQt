#pragma once

#include <QtWidgets/QMainWindow>
#include <QPushButton>
#include <QLabel>
#include <QVBoxLayout>
#include <QHBoxLayout>

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
    QPushButton* button1;
    QPushButton* button2;
    QLabel* labelOutput;
    QVBoxLayout* layout;
    QHBoxLayout* buttonLayout;
    QWidget* centralWidget;
};
