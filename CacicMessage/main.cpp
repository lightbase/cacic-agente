#include "cacicmessage.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
#ifdef Q_OS_LINUX
    a.setWindowIcon(QIcon("/usr/share/cacic/cacic-logo.png"));
#endif
    QStringList args;
    for (int i = 0; i<argc; i++){
        args.append(argv[i]);
    }
    CacicMessage w(args);
    QObject::connect(&w, SIGNAL(finished()), &a, SLOT(quit()));
    QMetaObject::invokeMethod(&w, "exibeMensagem", Qt::QueuedConnection);
    //w.show();

    return a.exec();
}
