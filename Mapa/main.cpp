#include "mapa.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    Mapa w;
    w.setArgs(argc, argv);
    w.show();

    return a.exec();
}
