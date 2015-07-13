#include <QApplication>
#include "mapacontrol.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    MapaControl *controller = new MapaControl(&a);
    controller->connect(controller, SIGNAL(finished()), &a, SLOT(quit()));
    QStringList args;
    for (int i = 0; i<argc; i++){
        args.append(argv[i]);
    }
    QMetaObject::invokeMethod(controller, "run", Qt::QueuedConnection, Q_ARG(QStringList, args));

    return a.exec();
}
