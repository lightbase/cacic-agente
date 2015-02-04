#include "socketlistener.h"

SocketListener::SocketListener(QString applicationDirPath, QObject *parent) :
    QTcpServer(parent)
{
    connect(&server_socket, SIGNAL(error(QAbstractSocket::SocketError)),
            this, SLOT(tcpError(QAbstractSocket::SocketError)) );
    connect(&server_socket, SIGNAL(readyRead()),
            this, SLOT(tcpReady()));
    connect(this, SIGNAL(newConnection()), this, SLOT(newRequest()));
    server_socket.setSocketOption(QAbstractSocket::KeepAliveOption, true);
    iniciarInstancias(applicationDirPath);
}

SocketListener::~SocketListener()
{
    server_socket.disconnectFromHost();
    server_socket.waitForDisconnected();
}

void SocketListener::tcpReady()
{
    QByteArray array = server_socket.read(server_socket.bytesAvailable());
}

void SocketListener::tcpError(QAbstractSocket::SocketError error)
{
    QLogger::QLog_Info(Identificadores::LOG_SOCKET_LISTENER, QString("TCP error: " + server_socket.errorString()));
}


bool SocketListener::start_listen(int port_no)
{
    if( !this->listen( QHostAddress::Any, port_no ) )
    {
        QLogger::QLog_Info(Identificadores::LOG_SOCKET_LISTENER, QString("Cannot listen to port " + port_no));
        return false;
    }
    else
        return true;

    this->newConnection();
}

bool SocketListener::newRequest()
{
    this->pendingConnection = this->nextPendingConnection();
    QLogger::QLog_Info(Identificadores::LOG_SOCKET_LISTENER, QString("Request peerAddress() : " + pendingConnection->peerAddress().toString()));
    QLogger::QLog_Info(Identificadores::LOG_SOCKET_LISTENER, QString("Request peerName() : " + pendingConnection->peerName()));
    QLogger::QLog_Info(Identificadores::LOG_SOCKET_LISTENER, QString("Request: " + QString::fromLocal8Bit(pendingConnection->readAll())));
    this->pendingConnection->close();

    return true;
}

void SocketListener::incomingConnection(int descriptor)
{
    if( !server_socket.setSocketDescriptor(descriptor))
    {
        QLogger::QLog_Info(Identificadores::LOG_SOCKET_LISTENER, QString("Socket error!"));
        return;
    }
}

void SocketListener::iniciarInstancias(QString applicationDirPath){
    logManager = QLogger::QLoggerManager::getInstance();
    logManager->addDestination(applicationDirPath + "/Logs/cacic.log",Identificadores::LOG_SOCKET_LISTENER, QLogger::InfoLevel);
    logManager->addDestination(applicationDirPath + "/Logs/cacic_error.log",Identificadores::LOG_SOCKET_LISTENER, QLogger::ErrorLevel);
}
