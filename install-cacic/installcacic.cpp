#include "installcacic.h"


QCoreApplication *InstallCacic::getApp() const
{
    return app;
}

void InstallCacic::setApp(const QCoreApplication &value)
{
    app = value;
}

InstallCacic::InstallCacic(QObject *parent) :
    QObject(parent)
{
}


void InstallCacic::runInstall()
{
    QString argumento;
    cout << "Testando argumentos" << endl;
    argumento = "--host";

    if (app.arguments().count() > 1){
        for(int i=1;i<app.arguments().count();i++){
            if (app.arguments().at(i) == argumento){
                app.quit();
            } else {
                cout << app.arguments().at(i).toStdString() << endl;
            }
        }
       app.exit(2);
    } else {
       app.exit(3);
    }
}


