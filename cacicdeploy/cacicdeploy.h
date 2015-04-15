/*-----------------------------------------------------------------------------------------
 *
 * Project Cacic Agente
 *    File created by Lightbase
 *
 * Developers: Eric Menezes Noronha (eric.m.noronha@lightbase.com.br); GitHub: ericmenezes
 *             Thiago Rocha         (thiago.rocha@lightbase.com.br)  ;
 *
 *-----------------------------------------------------------------------------------------*/
#ifndef CACICDEPLOY_H
#define CACICDEPLOY_H
#include <QCoreApplication>
#include <QEventLoop>
#include "qtservice.h"
#include "identificadores.h"
#include "ccacic.h"
//#include "socketlistener.h"
#include "logcacic.h"
#include <QTimer>
#include "deploytimer.h"

class cacicdeploy : public QtService<QCoreApplication>
{
public:
    //    QCoreApplication *app;

    cacicdeploy(int argc, char **argv);
    ~cacicdeploy();
    void start();
    void pause();
    void resume();
    void stop();

private:
    QString cacicFolder;
    LogCacic *logcacic;
    deployTimer *timer;

};
#endif // CACICD_H
