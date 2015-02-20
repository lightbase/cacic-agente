#ifndef CACICD_H
#define CACICD_H
#include <QCoreApplication>
#include <QEventLoop>
#include "qtservice.h"
#include "cacictimer.h"
#include "QLogger.h"
#include "checkmodules.h"
#include "identificadores.h"
#include "ccacic.h"
#include "socketlistener.h"

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
    QLogger::QLoggerManager *logManager;
    CCacic *ccacic;
};
#endif // CACICD_H
