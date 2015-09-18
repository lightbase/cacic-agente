#include "serviceserver.h"

ServiceServer::ServiceServer(const QString &dir,QObject *parent) :
    QTcpServer(parent),cacicMainFolder(dir)
{
    logcacic = new LogCacic(LOG_DAEMON,cacicMainFolder);

}

void ServiceServer::iniciarServer()
{
    if(!this->listen(QHostAddress::LocalHost,CACICDAEMON_PORT)){
        logcacic->escrever(LogCacic::ErrorLevel,"ServiceServer não pode ser iniciado.");
    } else {
        logcacic->escrever(LogCacic::InfoLevel,"ServiceServer iniciado.");
    }
}

void ServiceServer::incomingConnection(int socketDescriptor)
{
    logcacic->escrever(LogCacic::InfoLevel,"Conexão " + QString::number(socketDescriptor));
    QTcpSocket *socket = this->nextPendingConnection();

    ServerThread *thread = new ServerThread(socketDescriptor,cacicMainFolder,this);
    connect(thread,&ServerThread::finished,thread,&ServerThread::deleteLater);
    thread->start();
}
