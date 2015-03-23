#include "deploytimer.h"

deployTimer::deployTimer(QObject *parent) :
    QObject(parent)
{
}

deployTimer::deployTimer(CCacic *cacic){
    oCacic = cacic;
    log = new LogCacic(LOG_CACICDEPLOY, oCacic->getCacicMainFolder()+"/Logs");
    timerDeploy = new QTimer();
    QObject::connect(timerDeploy, SIGNAL(timeout()), this, SLOT(onTimer()));
}

bool deployTimer::start(int msec)
{
    timerDeploy->setInterval(msec);
    timerDeploy->start();
    return true;
}

void deployTimer::onTimer()
{
    log->escrever(LogCacic::InfoLevel, "OnTimer");
}
