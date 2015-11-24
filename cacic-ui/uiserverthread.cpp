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

    exec();
}

void UiServerThread::disconnected()
{
    socket->deleteLater();
    exit(0);
}

void UiServerThread::sendAck()
{
    QByteArray data;
    data.append(QString::number(MSG_LENGTH_ACK));
    data.append(" ");
    data.append(MSG_ACK);
    data.append(MSG_END);
    socket->write(data);
}

void UiServerThread::parseData(const QString &dataReceived)
{
    QStringList splitData = dataReceived.split(" ");

    if(splitData.size() > 1) {
        QString numberOfChars = splitData.at(0);

        bool ok;
        int messageLength = numberOfChars.toInt(&ok);
        if(ok) {
            QString message = dataReceived.mid(numberOfChars.size()+1,messageLength);
            if( message == MSG_DAEMONSTOPUI ) {
                sendAck();
                emit finalizarUi();
            }
        }
    }
}

void UiServerThread::readyRead()
{
    QString dataReceived(socket->readLine());

    logcacic->escrever(LogCacic::InfoLevel,QString::number(socketDescriptor)
                       + " Data received: "
                       + dataReceived);

    parseData(dataReceived);
}
