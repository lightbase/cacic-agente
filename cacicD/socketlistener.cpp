#include "socketlistener.h"

SocketListener::SocketListener(QString DirPath, QObject *parent) :
    QObject(parent)
{
    this->applicationDirPath = DirPath;
    server = new QLocalServer(this);
    connect(server, SIGNAL(newConnection()), this, SLOT(newConnection()));
    logcacic = new LogCacic(LOG_SOCKET_LISTENER, DirPath+"/Logs");
    if(!server->listen(CACIC_SERVICE_NAME)) {
        logcacic->escrever(LogCacic::ErrorLevel, QString("Erro ao iniciar escuta de socket."));
    }
}

void SocketListener::newConnection() {
    QLocalSocket *socket = server->nextPendingConnection();
    socket->write("Pedido para coleta forçada recebido com sucesso.\n\n");
    logcacic->escrever(LogCacic::InfoLevel, QString("Pedido para coleta forçada recebido: " +
                                                        QString::fromLocal8Bit(socket->readAll())));
    socket->flush();
    socket->waitForBytesWritten(3000);
    socket->close();
    emit forcaColeta();
}

SocketListener::~SocketListener(){
    server->close();
}
