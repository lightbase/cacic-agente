#include "serverthread.h"

ServerThread::ServerThread(const int &sockDescriptor, const QString &dir, QObject *parent) : QThread(parent)
{
    socketDescriptor = sockDescriptor;
    cacicMainFolder = dir;

    logcacic = new LogCacic(LOG_DAEMON_TIMER,cacicMainFolder+"/Logs");
}

void ServerThread::run()
{
    socket = new QTcpSocket();
    if(!socket->setSocketDescriptor(socketDescriptor)){
        logcacic->escrever(LogCacic::ErrorLevel,"SocketDescriptor "
                           +QString::number(socketDescriptor)
                           +" already taken.");
        return;
    }

    connect(socket,&QTcpSocket::readyRead,this,&ServerThread::readyRead,Qt::DirectConnection);
    connect(socket,&QTcpSocket::disconnected,this,&ServerThread::disconnected,Qt::DirectConnection);

    logcacic->escrever(LogCacic::InfoLevel,"ServerThread "+QString::number(socketDescriptor));

    exec();
}

void ServerThread::disconnected()
{
    socket->deleteLater();
    exit(0);
}

void ServerThread::readyRead()
{
    logcacic->escrever(LogCacic::InfoLevel,QString::number(socketDescriptor)
                       + "Data received: "
                       + QString(socket->readAll()));
}
