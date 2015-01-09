#include "installcacicgui.h"
#include "noroot.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    InstallCacicGui *w = new InstallCacicGui();
    if(w->verificarRoot()){
        w->parent()->connect(w, SIGNAL(finished()), &a, SLOT(quit()));
        if(argc > 1){
            QStringList args;
            for (int i = 0; i < argc; i++){
                args.append(argv[i]);
            }
            w->setGui(false);
            QMetaObject::invokeMethod(w, "run", Qt::QueuedConnection, Q_ARG(QStringList, args), Q_ARG(int, argc));
        }else{
            w->setGui(true);
            w->show();
        }
    }else{
        if(argc > 1){
            w->parent()->connect(w, SIGNAL(finished()), &a, SLOT(quit()));
            QStringList args;
            for (int i = 0; i < argc; i++){
                args.append(argv[i]);
            }
            w->setGui(false);
            QMetaObject::invokeMethod(w, "run", Qt::QueuedConnection, Q_ARG(QStringList, args), Q_ARG(int, argc));
        }else{
            NoRoot *noRoot = new NoRoot();
            noRoot->parent()->connect(noRoot, SIGNAL(finished()), &a, SLOT(quit()));
            noRoot->show();
        }
    }
    return a.exec();
}
