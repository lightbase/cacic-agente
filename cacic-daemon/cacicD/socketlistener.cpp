#include "socketlistener.h"

SocketListener::SocketListener(QString DirPath, QObject *parent) :
    QTcpServer(parent)
{
    this->applicationDirPath = DirPath;
    connect(&server_socket, SIGNAL(error(QAbstractSocket::SocketError)),
            this, SLOT(tcpError(QAbstractSocket::SocketError)) );
    connect(&server_socket, SIGNAL(readyRead()),
            this, SLOT(tcpReady()));
    connect(this, SIGNAL(newConnection()), this, SLOT(newRequest()));
    server_socket.setSocketOption(QAbstractSocket::KeepAliveOption, true);
    iniciarInstancias();
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
    if(!this->listen(QHostAddress::Any, port_no))
    {
        QLogger::QLog_Info(Identificadores::LOG_SOCKET_LISTENER, QString("Não foi possivel iniciar escuta na porta: " + QString::number(port_no)));
        return false;
    }else{
        QLogger::QLog_Info(Identificadores::LOG_SOCKET_LISTENER, QString("Escuta na porta " + QString::number(port_no) + " iniciada com sucesso."));
        return true;
    }
}

bool SocketListener::newRequest()
{
    try{
        this->pendingConnection = this->nextPendingConnection();
        QLogger::QLog_Info(Identificadores::LOG_SOCKET_LISTENER, QString("Ordem para forçar coleta recebida de: " + pendingConnection->peerAddress().toString()));
        iniciarColetaForcada();
        this->pendingConnection->close();
        return true;
    }catch (...) {
        QLogger::QLog_Info(Identificadores::LOG_SOCKET_LISTENER, QString("Erro ao receber socket request."));
        return false;
    }
}

void SocketListener::incomingConnection(int descriptor)
{
    if(!server_socket.setSocketDescriptor(descriptor))
    {
        QLogger::QLog_Info(Identificadores::LOG_SOCKET_LISTENER, QString("Socket error!"));
        return;
    }
}

void SocketListener::iniciarInstancias(){
    logManager = QLogger::QLoggerManager::getInstance();
    logManager->addDestination(this->applicationDirPath + "/Logs/cacic.log",Identificadores::LOG_SOCKET_LISTENER, QLogger::InfoLevel);
    logManager->addDestination(this->applicationDirPath + "/Logs/cacic_error.log",Identificadores::LOG_SOCKET_LISTENER, QLogger::ErrorLevel);
}

void SocketListener::iniciarColetaForcada(){

}
