#ifndef UISERVER_H
#define UISERVER_H

#include <QString>
#include <QTcpServer>
#include "uiserverthread.h"
#include "../src/logcacic.h"
#include "../src/identificadores.h"

class UiServer : public QTcpServer
{
    Q_OBJECT
public:
    explicit UiServer(const QString &dir,QObject *parent = 0);
    void iniciarServer();

signals:
    void finalizar();

public slots:

protected:
    void incomingConnection(int socketDescriptor);

private:
    LogCacic *logcacic;
    QString cacicMainFolder;

};

#endif // UISERVER_H
