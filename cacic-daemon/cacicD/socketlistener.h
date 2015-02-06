#ifndef SOCKETLISTENER_H
#define SOCKETLISTENER_H

#include <QObject>
#include <QtNetwork>
#include "identificadores.h"
#include "QLogger.h"
#include "cacicthread.h"
#include "cacic_comm.h"
#include "ccacic.h"

class SocketListener : public QTcpServer
{
    Q_OBJECT
public:
    explicit SocketListener(QString DirPath, QObject *parent = 0);
    ~SocketListener();
    void setApplicationDirPath(const QString &value);

private:
    QTcpSocket server_socket;
    QTcpSocket *pendingConnection;
    QLogger::QLoggerManager *logManager;
    QString applicationDirPath;
    void iniciarInstancias();
    void iniciarColetaForcada();

public slots:
    void tcpReady();
    void tcpError(QAbstractSocket::SocketError error);
    bool start_listen(int port_no);
    bool newRequest();

protected:
    void incomingConnection(int descriptor);

};

#endif // SOCKETLISTENER_H
