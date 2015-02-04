#ifndef SOCKETLISTENER_H
#define SOCKETLISTENER_H

#include <QObject>
#include <QtNetwork>
#include "identificadores.h"
#include "QLogger.h"

class SocketListener : public QTcpServer
{
    Q_OBJECT
public:
    explicit SocketListener(QString applicationDirPath, QObject *parent = 0);
    ~SocketListener();
    QTcpSocket server_socket;
    QTcpSocket *pendingConnection;
    void setApplicationDirPath(const QString &value);

private:
    QLogger::QLoggerManager *logManager;
    void iniciarInstancias(QString applicationDirPath);

public slots:
    void tcpReady();
    void tcpError(QAbstractSocket::SocketError error);
    bool start_listen(int port_no);
    bool newRequest();

protected:
    void incomingConnection(int descriptor);

};

#endif // SOCKETLISTENER_H
