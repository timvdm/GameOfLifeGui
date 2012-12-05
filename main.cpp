#include <QtGui/QApplication>
#include "gameoflifegui.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    GameOfLifeGui w;
    w.show();

    return a.exec();
}
