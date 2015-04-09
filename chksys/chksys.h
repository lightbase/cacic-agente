#ifndef CHKSYS_H
#define CHKSYS_H
#include <QCoreApplication>
#include <QEventLoop>
#include "qtservice.h"
#include "identificadores.h"
#include "ccacic.h"
//#include "socketlistener.h"
#include "logcacic.h"
#include <QTimer>
#include "chksystimer.h"

class chksys : public QtService<QCoreApplication>
{
public:
    //    QCoreApplication *app;

    chksys(int argc, char **argv);
    ~chksys();
    void start();
    void pause();
    void resume();
    void stop();

private:
    QString cacicFolder;
    LogCacic *logcacic;
    chksysTimer *timer;

};
#endif // CACICD_H
