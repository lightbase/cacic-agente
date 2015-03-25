#ifndef CACICD_H
#define CACICD_H
#include <QCoreApplication>
#include <QEventLoop>
#include "qtservice.h"
#include "cacictimer.h"
#include "identificadores.h"
#include "ccacic.h"
#include "socketlistener.h"
#include <qlocalserver.h>
#include "logcacic.h"

class cacicD : public QtService<QCoreApplication>
{
public:
    //    QCoreApplication *app;

    cacicD(int argc, char **argv);
    ~cacicD();
    void start();
    void pause();
    void resume();
    void stop();

private:
    QLocalServer *serverListener;
    QString cacicMainFolder;
    LogCacic *logcacic;

};
#endif // CACICD_H
