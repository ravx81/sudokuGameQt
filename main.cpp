#include <QApplication>
#include <QFile>
#include "sudokuGameQt.h"

int main(int argc, char* argv[])
{
    QApplication app(argc, argv);

    QFile file("styles/app.qss");
    if (file.open(QFile::ReadOnly)) {
        QString styleSheet = QLatin1String(file.readAll());
        app.setStyleSheet(styleSheet);
        file.close();
    }

    sudokuGameQt window;
    window.show();

    return app.exec();
}
