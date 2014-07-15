#include <QtCore>
#include <QCoreApplication>
#include "installcacic.h"

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    InstallCacic *installcacic = new InstallCacic(&a);

    // This will cause the application to exit when
    // the task signals finished.
    QObject::connect(installcacic, SIGNAL(finished(int)), &a, SLOT(quit()));

    // This will run the task from the application event loop.
    QTimer::singleShot(0, installcacic, SLOT(run()));

    return a.exec();
}
