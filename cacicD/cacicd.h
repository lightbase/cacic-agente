/*-----------------------------------------------------------------------------------------
 *
 * Project Cacic Agente
 *    File created by Lightbase
 *
 * Developers: Eric Menezes Noronha (eric.m.noronha@lightbase.com.br); GitHub: ericmenezes
 *             Thiago Rocha         (thiago.rocha@lightbase.com.br)  ;
 *
 *-----------------------------------------------------------------------------------------*/
#ifndef CACICD_H
#define CACICD_H
#include <QCoreApplication>
#include <QEventLoop>
#include <QMutex>
#include <QWaitCondition>
#include "qtservice.h"
#include "cacictimer.h"
#include "identificadores.h"
#include "ccacic.h"
#include "socketlistener.h"
#include <qlocalserver.h>
#include "logcacic.h"
#include "serviceclient.h"

class cacicD : public QObject, public QtService<QCoreApplication>
{
    Q_OBJECT
public:
    //    QCoreApplication *app;

    cacicD(int argc, char **argv);
    ~cacicD();
    void start();
    void pause();
    void resume();
    void stop();

signals:
    void sendStopUiMsg();

private slots:
    void on_uiStopped();
    void on_finalizar();

private:
    QByteArray formatData(QString message, int messageLength);

    QMutex *mutex;
    QWaitCondition *ackReceived;
    QLocalServer *serverListener;
    QString cacicMainFolder;
    LogCacic *logcacic;

};
#endif // CACICD_H
