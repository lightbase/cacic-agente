#include "socketlistener.h"

SocketListener::SocketListener(QString DirPath, QObject *parent) :
    QObject(parent)
{
    this->applicationDirPath = DirPath;
    server = new QTcpServer(this);
    connect(server, SIGNAL(newConnection()), this, SLOT(newConnection()));
    logcacic = new LogCacic(Identificadores::LOG_SOCKET_LISTENER, DirPath+"/Logs");
    if(!server->listen(QHostAddress::Any, this->port_no)) {
        logcacic->escrever(LogCacic::ErrorLevel, QString("Erro ao iniciar escuta de socket."));
    }else{
        logcacic->escrever(LogCacic::InfoLevel, QString("Escuta na porta " +
                                                        QString::number(this->port_no) +
                                                        " iniciado com sucesso."));
    }
}

void SocketListener::newConnection() {
    QTcpSocket *socket = server->nextPendingConnection();
    socket->write("Pedido para coleta forçada recebido com sucesso.\n\n");
    logcacic->escrever(LogCacic::InfoLevel, QString("Pedido para coleta forçada recebido de: " + socket->localAddress().toString()));
    socket->flush();
    socket->waitForBytesWritten(3000);
    socket->close();
    emit forcaColeta();
}

void SocketListener::setPort_no(int value) {
    port_no = value;
}

SocketListener::~SocketListener(){
    server->close();
}
