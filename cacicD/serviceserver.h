#ifndef SERVICESERVER_H
#define SERVICESERVER_H

#include <QString>
#include <QTcpServer>
#include "serviceserverthread.h"
#include "../src/logcacic.h"
#include "../src/identificadores.h"

class ServiceServer : public QTcpServer
{
    Q_OBJECT
public:
    explicit ServiceServer(const QString &dir,QObject *parent = 0);
    void iniciarServer();

signals:
    void forcarColeta();
    void finalizarCacic();
public slots:

protected:
    void incomingConnection(int socketDescriptor);

private:
    LogCacic *logcacic;
    QString cacicMainFolder;

};

#endif // SERVICESERVER_H
