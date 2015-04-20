/*-----------------------------------------------------------------------------------------
 *
 * Project Cacic Agente
 *    File created by Lightbase
 *
 * Developers: Eric Menezes Noronha (eric.m.noronha@lightbase.com.br); GitHub: ericmenezes
 *             Thiago Rocha         (thiago.rocha@lightbase.com.br)  ;
 *
 *-----------------------------------------------------------------------------------------*/
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
#include <cacicthread.h>

class deployTimer : public QObject
{
    Q_OBJECT
private:
    LogCacic *log;
    QTimer *timerDeploy;
    QTimer *timerCheckService;
    QString cacicFolder;
    QMutex *mutex;
    int timeout;
    QJsonObject moduloExec;
    CacicThread *thread;

    bool commExecucao(QJsonObject modulo, QString rota, bool statusExec = false);
    bool downloadModulo(QString nome);
public:
    explicit deployTimer(QObject *parent = 0);
    explicit deployTimer(QString cacicFolder);
    bool start(int msecDeploy, int msecCheckService);
    int getTimeout() const;
    void setTimeout(int value);

signals:

public slots:
    void onTimer();
    void onTimerCheckService();

private slots:
    void confirmaExecucao();

};

#endif // DEPLOYTIMER_H
