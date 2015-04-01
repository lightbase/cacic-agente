#include "mapa.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    Mapa w;
    w.show();

    return a.exec();
}
