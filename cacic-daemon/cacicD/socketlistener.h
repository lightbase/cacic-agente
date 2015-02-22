#ifndef SOCKETLISTENER_H
#define SOCKETLISTENER_H

#include <QObject>
#include <QTcpServer>
#include <QTcpSocket>
#include "QLogger.h"
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

public slots:
    void newConnection();

private:
    QTcpServer *server;
    QLogger::QLoggerManager *logManager;
    QString applicationDirPath;
    CacicThread *cacicthread;
    CCacic *ccacic;
    CacicComm *oCacicComm;
    QMutex *cMutex;
    int port_no = 1500; //porta padrão
    void iniciarInstancias();
    void iniciarColetaForcada();
};

#endif // SOCKETLISTENER_H
