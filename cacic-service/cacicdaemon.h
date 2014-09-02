#ifndef CACICDAEMON_H
#define CACICDAEMON_H

#include <QCoreApplication>
#include <QDebug>
#include <QMutex>
#include <QThread>
#include <QWaitCondition>
#include <stdlib.h>
#include <unistd.h>


class CacicDaemon : public QThread
{
    Q_OBJECT
public:
    CacicDaemon(QObject* parent = 0);
    ~CacicDaemon();

    void pause();
    void resume();

protected:
    void run();

private slots:

private:
    bool getTest();
    bool getConfig();

    QMutex mutex;
    QWaitCondition condition;
    bool abort;
    bool paused;

};

#endif // CACICDAEMON_H
