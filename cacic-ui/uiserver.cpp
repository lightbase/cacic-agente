#include "uiserver.h"

UiServer::UiServer(const QString &dir,QObject *parent) :
    QTcpServer(parent),cacicMainFolder(dir)
{
    logcacic = new LogCacic(LOG_CACICUI,cacicMainFolder + "/Logs");

}

void UiServer::iniciarServer()
{
    if(!this->listen(QHostAddress::LocalHost,PORT_CACICUI)){
        logcacic->escrever(LogCacic::ErrorLevel,"UiServer não pode ser iniciado.");
    } else {
        logcacic->escrever(LogCacic::InfoLevel,"UiServer iniciado.");
    }
}

void UiServer::incomingConnection(int socketDescriptor)
{
    logcacic->escrever(LogCacic::InfoLevel,"Conexão " + QString::number(socketDescriptor));
    QTcpSocket *socket = this->nextPendingConnection();

    UiServerThread *thread = new UiServerThread(socketDescriptor,cacicMainFolder,this);
    connect(thread,&UiServerThread::finished,thread,&UiServerThread::deleteLater);
    thread->start();
}
