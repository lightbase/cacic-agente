#include "uiserverthread.h"

UiServerThread::UiServerThread(const int &sockDescriptor, const QString &dir, QObject *parent) : QThread(parent)
{
    socketDescriptor = sockDescriptor;
    cacicMainFolder = dir;

    logcacic = new LogCacic(LOG_CACICUI,cacicMainFolder+"/Logs");
}

void UiServerThread::run()
{
    socket = new QTcpSocket();
    if(!socket->setSocketDescriptor(socketDescriptor)){
        logcacic->escrever(LogCacic::ErrorLevel,"SocketDescriptor "
                           +QString::number(socketDescriptor)
                           +" already taken.");
        return;
    }

    connect(socket,&QTcpSocket::readyRead,this,&UiServerThread::readyRead,Qt::DirectConnection);
    connect(socket,&QTcpSocket::disconnected,this,&UiServerThread::disconnected,Qt::DirectConnection);

    logcacic->escrever(LogCacic::InfoLevel,"UiServerThread "+QString::number(socketDescriptor));

    exec();
}

void UiServerThread::disconnected()
{
    socket->deleteLater();
    exit(0);
}

void UiServerThread::readyRead()
{
    logcacic->escrever(LogCacic::InfoLevel,QString::number(socketDescriptor)
                       + " Data received: "
                       + QString(socket->readLine()));
}
