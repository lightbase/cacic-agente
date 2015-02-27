#ifndef SOCKETLISTENER_H
#define SOCKETLISTENER_H

#include <QObject>
#include <QTcpServer>
#include <QTcpSocket>
#include "logcacic.h"
#include "identificadores.h"
#include "cacicthread.h"
#include "ccacic.h"
#include "cacic_comm.h"

class SocketListener : public QObject
{
    Q_OBJECT
public:
    explicit SocketListener(QString DirPath, QObject *parent = 0);
    ~SocketListener();
    void setPort_no(int value);

signals:
    void forcaColeta();
public slots:
    void newConnection();

private:
    QTcpServer *server;
    LogCacic *logcacic;
    QString applicationDirPath;
    CacicThread *cacicthread;
    CCacic *ccacic;
    CacicComm *oCacicComm;
    QMutex *cMutex;
    int port_no = 1500; //porta padrão
    void iniciarColetaForcada();
};

#endif // SOCKETLISTENER_H
