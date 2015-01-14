#include "installcacicgui.h"
#include "noroot.h"
#include "uninstallcacic.h"
#include <QApplication>
#include "ccacic.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    InstallCacicGui *w = new InstallCacicGui();
    w->setWindowIcon(QIcon(":/CacicFiles/cacic-logo.png"));
    CCacic *oCaic = new CCacic();
    if(oCaic->verificarRoot()){
        w->parent()->connect(w, SIGNAL(finished()), &a, SLOT(quit()));
        if(argc > 1){
            QStringList args;
            for (int i = 0; i < argc; i++){
                args.append(argv[i]);
            }
            w->setGui(false);
            QMetaObject::invokeMethod(w, "run", Qt::QueuedConnection, Q_ARG(QStringList, args), Q_ARG(int, argc));
        }else{
            if(oCaic->verificarCacicInstalado()){
                UninstallCacic *uni = new UninstallCacic();
                uni->setWindowIcon(QIcon(":/CacicFiles/cacic-logo.png"));
                uni->parent()->connect(uni, SIGNAL(finished()), &a, SLOT(quit()));
                uni->setObjInstallCacic(w);
                uni->show();
            }else{
                w->setGui(true);
                w->setModoDeExecucao(Identificadores::INSTALAR);
                w->show();
            }
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
            noRoot->setWindowIcon(QIcon(":/CacicFiles/cacic-logo.png"));
            noRoot->parent()->connect(noRoot, SIGNAL(finished()), &a, SLOT(quit()));
            noRoot->show();
        }
    }
    return a.exec();
}
