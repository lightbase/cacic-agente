#ifndef DEPLOYTIMER_H
#define DEPLOYTIMER_H

#include <QObject>
#include <QTimer>
#include <logcacic.h>
#include <ccacic.h>

class deployTimer : public QObject
{
    Q_OBJECT
private:
    LogCacic *log;
    CCacic *oCacic;
    QTimer *timerDeploy;
public:
    explicit deployTimer(QObject *parent = 0);
    explicit deployTimer(CCacic *cacic);
    bool start(int msec);
signals:

public slots:
    void onTimer();

};

#endif // DEPLOYTIMER_H
