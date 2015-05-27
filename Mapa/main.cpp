#include <QApplication>
#include "mapacontrol.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    MapaControl *controller = new MapaControl(&a);
    controller->connect(controller, SIGNAL(finished()), &a, SLOT(quit()));

    controller->run(argc, argv);

    return a.exec();
}
