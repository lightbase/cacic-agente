#include <QCoreApplication>
#include <gercols.h>

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    QStringList args;
    for (int i = 0; i<argc; i++)
        args.append(argv[i]);

    Gercols *OGercols = new Gercols(&a);

    // This will cause the application to exit when
    // the task signals finished.
    QObject::connect(OGercols, SIGNAL(finished()), &a, SLOT(quit()));

    // This will run the task from the application event loop.
    QMetaObject::invokeMethod(OGercols, "run", Qt::QueuedConnection, Q_ARG(QStringList, args), Q_ARG(int, argc));
    return a.exec();
}
