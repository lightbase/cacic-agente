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
#include "trayicon.h"

#define APP_VERSION "3.0"

class cacicD : public QtService<QCoreApplication>
{
public:
//    QCoreApplication *app;
    QLogger::QLoggerManager *logManager;
    CacicTimer *Ocacictimer;
    CCacic *ccacic;
    CheckModules *OcheckModules;

    cacicD(int argc, char **argv);
    ~cacicD();
    void start();
    void pause();
    void resume();
    void stop();

};
#endif // CACICD_H
