#include "sudokuGameQt.h"
#include <QApplication>

int main(int argc, char* argv[])
{
    QApplication app(argc, argv);

    sudokuGameQt window;
    window.show();

    return app.exec();
}
