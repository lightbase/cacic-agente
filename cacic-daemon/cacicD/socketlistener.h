#ifndef SOCKETLISTENER_H
#define SOCKETLISTENER_H

#include <QObject>
#include <QtNetwork>

class SocketListener : public QTcpServer
{
    Q_OBJECT
public:
    explicit SocketListener(QObject *parent = 0);
    ~SocketListener();
    QTcpSocket server_socket;

public slots:
    void tcpReady();
    void tcpError(QAbstractSocket::SocketError error);
    bool start_listen(int port_no);

protected:
    void incomingConnection(int descriptor);

};

#endif // SOCKETLISTENER_H
