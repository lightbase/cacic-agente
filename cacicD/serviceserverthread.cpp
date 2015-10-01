#include "serviceserverthread.h"

ServiceServerThread::ServiceServerThread(const int &sockDescriptor, const QString &dir, QObject *parent) : QThread(parent)
{
    socketDescriptor = sockDescriptor;
    cacicMainFolder = dir;

    logcacic = new LogCacic(LOG_DAEMON_TIMER,cacicMainFolder+"/Logs");
}

void ServiceServerThread::run()
{
    socket = new QTcpSocket();
    if(!socket->setSocketDescriptor(socketDescriptor)){
        logcacic->escrever(LogCacic::ErrorLevel,"SocketDescriptor "
                           +QString::number(socketDescriptor)
                           +" already taken.");
        return;
    }

    connect(socket,&QTcpSocket::readyRead,this,&ServiceServerThread::readyRead,Qt::DirectConnection);
    connect(socket,&QTcpSocket::disconnected,this,&ServiceServerThread::disconnected,Qt::DirectConnection);

    logcacic->escrever(LogCacic::InfoLevel,"ServerThread "+QString::number(socketDescriptor));

    exec();
}

void ServiceServerThread::disconnected()
{
    socket->deleteLater();
    exit(0);
}

//void ServiceServerThread::sendAck()
//{
//    QByteArray data;
//    data.append(QString::number(MSG_LENGTH_ACK));
//    data.append(" ");
//    data.append(MSG_ACK);
//    data.append(MSG_END);
//    socket->write(data);
//}

void ServiceServerThread::parseData(const QString &dataReceived)
{
    QStringList splitData = dataReceived.split(" ");

    if(splitData.size() > 1) {
        QString numberOfChars = splitData.at(0);
logcacic->escrever(LogCacic::InfoLevel,"parseData numberOfChars: " + numberOfChars);

        bool ok;
        int messageLength = numberOfChars.toInt(&ok);
        if(ok) {
            QString message = dataReceived.mid(numberOfChars.size()+1,messageLength);
logcacic->escrever(LogCacic::InfoLevel,"parseData message: " + message);
            if( message == MSG_UIFORCAR ) {
logcacic->escrever(LogCacic::InfoLevel,"parseData message FORCAR COLETA");
                emit forcarColeta();
//                sendAck();
            } else if( message == MSG_UIFINALIZAR ) {
logcacic->escrever(LogCacic::InfoLevel,"parseData message FINALIZAR CACIC");
                emit finalizarCacic();
//                sendAck();
            }
        }
    }
}

void ServiceServerThread::readyRead()
{
    QString dataReceived(socket->readLine());

    logcacic->escrever(LogCacic::InfoLevel,QString::number(socketDescriptor)
                       + " Data received: "
                       + dataReceived);

    parseData(dataReceived);

}
