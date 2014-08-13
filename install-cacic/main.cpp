#include <QtCore>
#include <QCoreApplication>
#include "installcacic.h"

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
    QStringList args;
    for (int i = 0; i<argc; i++)
        args.append(argv[i]);
    InstallCacic *installcacic = new InstallCacic(&a);

    // This will cause the application to exit when
    // the task signals finished.
    QObject::connect(installcacic, SIGNAL(finished()), &a, SLOT(quit()));

    // This will run the task from the application event loop.
    QMetaObject::invokeMethod(installcacic, "run", Qt::QueuedConnection, Q_ARG(QStringList, args), Q_ARG(int, argc));
    return a.exec();
}
