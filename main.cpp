#include "sudokuGameQt.h"
#include <QtWidgets/QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    sudokuGameQt w;
    w.show();
    return a.exec();
}
