#include "serviceserver.h"

ServiceServer::ServiceServer(const QString &dir,QObject *parent) :
    QTcpServer(parent),cacicMainFolder(dir)
{
    logcacic = new LogCacic(LOG_DAEMON,cacicMainFolder + "/Logs");

}

void ServiceServer::iniciarServer()
{
    if(!this->listen(QHostAddress::LocalHost,PORT_CACICDAEMON)){
        logcacic->escrever(LogCacic::ErrorLevel,"ServiceServer não pode ser iniciado.");
    } else {
        logcacic->escrever(LogCacic::InfoLevel,"ServiceServer iniciado.");
    }
}

void ServiceServer::incomingConnection(int socketDescriptor)
{
    logcacic->escrever(LogCacic::InfoLevel,"Conexão " + QString::number(socketDescriptor));
    QTcpSocket *socket = this->nextPendingConnection();

    ServiceServerThread *thread = new ServiceServerThread(socketDescriptor,cacicMainFolder,this);

    connect(thread,&ServiceServerThread::finished,thread,&ServiceServerThread::deleteLater);
    connect(thread,&ServiceServerThread::forcarColeta,this,&ServiceServer::forcarColeta);
    connect(thread,&ServiceServerThread::finalizarCacic,this,&ServiceServer::finalizarCacic);
    thread->start();
}
