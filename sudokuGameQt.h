#pragma once

#include <QtWidgets/QMainWindow>
#include "ui_sudokuGameQt.h"

class sudokuGameQt : public QMainWindow
{
    Q_OBJECT

public:
    sudokuGameQt(QWidget *parent = nullptr);
    ~sudokuGameQt();

private:
    Ui::sudokuGameQtClass ui;
};
