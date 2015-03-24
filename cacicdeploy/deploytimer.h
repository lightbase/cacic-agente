#ifndef DEPLOYTIMER_H
#define DEPLOYTIMER_H

#include <QObject>
#include <QTimer>
#include <logcacic.h>
#include <ccacic.h>
#include <qtunixsocket.h>
#include <QDateTime>
#include <QJsonArray>
#include <QJsonObject>
#include <QJsonValue>
#include <cacic_computer.h>
#include <cacic_comm.h>

class deployTimer : public QObject
{
    Q_OBJECT
private:
    LogCacic *log;
    CCacic *oCacic;
    QTimer *timerDeploy;
    QTimer *timerCheckService;

    bool autorizaExecucao(QString rota, bool statusExec);
public:
    explicit deployTimer(QObject *parent = 0);
    explicit deployTimer(CCacic *cacic);
    bool start(int msecDeploy, int msecCheckService);
signals:

public slots:
    void onTimer();
    void onTimerCheckService();

};

#endif // DEPLOYTIMER_H
