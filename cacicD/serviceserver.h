#ifndef SERVICESERVER_H
#define SERVICESERVER_H

#include <QString>
#include <QTcpServer>
#include "serverthread.h"
#include "../src/logcacic.h"
#include "../src/identificadores.h"

class ServiceServer : public QTcpServer
{
    Q_OBJECT
public:
    explicit ServiceServer(const QString &dir,QObject *parent = 0);
    void iniciarServer();

signals:

public slots:

protected:
    void incomingConnection(int socketDescriptor);

private:
    LogCacic *logcacic;
    QString cacicMainFolder;

};

#endif // SERVICESERVER_H
