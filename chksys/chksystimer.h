#ifndef CHKSYSTIMER_H
#define CHKSYSTIMER_H

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

class chksysTimer : public QObject
{
    Q_OBJECT
private:
    LogCacic *log;
    QTimer *timerCheckService;
    QString cacicFolder;

    bool downloadService();

public:
    explicit chksysTimer(QObject *parent = 0);
    explicit chksysTimer(QString cacicFolder);
    bool start(int msecCheckService);

signals:

public slots:
    void onTimerCheckService();
};

#endif // CHKSYSTIMER_H
