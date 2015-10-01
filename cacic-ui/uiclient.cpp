#include "uiclient.h"

UiClient::UiClient(const QString &dirpath, QWidget *parent):
    QWidget(parent)
{
    if(!dirpath.isEmpty()) {
        mainFolder = dirpath;
        logcacic = new LogCacic(LOG_CACICUI, mainFolder+"/Logs");
    } else {
        logcacic = new LogCacic(LOG_CACICUI, Identificadores::ENDERECO_PATCH_CACIC+"/Logs");
    }

    connected = false;
    setupSocketConnection();
}

bool UiClient::isConnected()
{
    return connected;
}

void UiClient::on_bytesWritten(qint64 bytes)
{

}

void UiClient::on_connected()
{
    connected = true;
    logcacic->escrever(LogCacic::InfoLevel,"Socket conectado.");
}

void UiClient::on_disconnected()
{
    connected = false;
    logcacic->escrever(LogCacic::InfoLevel,"Socket desconectado.");
}

void UiClient::on_forcarClicked()
{
    // TODO: enviar comando para forçar coleta no serviço
    if(connected) {

        QByteArray data;
        data.append(QString::number(MSG_LENGTH_UIFORCAR));
        data.append(" ");
        data.append(MSG_UIFORCAR);
        data.append(MSG_END);

        socket->write(data);
//        timeout2Ack();
    } else
        logcacic->escrever(LogCacic::ErrorLevel,QString("forcarClicked: socket disconnected."));

}

void UiClient::on_finalizarClicked()
{
    // TODO: enviar comando para finalizar os serviços do Cacic
    if(connected) {

        QByteArray data;
        data.append(QString::number(MSG_LENGTH_UIFINALIZAR));
        data.append(" ");
        data.append(MSG_UIFINALIZAR);
        data.append(MSG_END);

        socket->write(data);
//        timeout2Ack(data);
    } else
        logcacic->escrever(LogCacic::ErrorLevel,QString("finalizarClicked: socket disconnected."));
}

void UiClient::on_readyRead()
{
    QString dataReceived(socket->readLine());
    parseData(dataReceived);
}

void UiClient::parseData(const QString &dataReceived)
{
    QStringList splitData = dataReceived.split(" ");

    if(splitData.size() > 1) {
        QString numberOfChars = splitData.at(0);

        bool ok;
        int messageLength = numberOfChars.toInt(&ok);
        if(ok) {
            QString message = dataReceived.mid(numberOfChars.size()+1,messageLength);
            if( message == MSG_ACK ) {
//                stopTimer();
            }
        }
    }
}

void UiClient::setupSocketConnection()
{
    socket = new QTcpSocket(this);

    connect(socket,&QTcpSocket::connected,this,&UiClient::on_connected);
    connect(socket,&QTcpSocket::disconnected,this,&UiClient::on_disconnected);
    connect(socket,&QTcpSocket::readyRead,this,&UiClient::on_readyRead);
    connect(socket,&QTcpSocket::bytesWritten,this,&UiClient::on_bytesWritten);

    socket->connectToHost("127.0.0.1",PORT_CACICDAEMON);

    if(!socket->waitForConnected(1000)) {
        logcacic->escrever(LogCacic::ErrorLevel, "Erro ao conectar ao Cacic-service.");
    }

}

//void UiClient::stopTimer()
//{
//    // Para o timer e limpa o timeoutBuffer
//}

//void UiClient::timeout2Ack(QByteArray& dataSent)
//{
//    timeoutBuffer = dataSent;
//    //Seta um timer com um pequeno timeout para receber um ACK,
//    //se acabar o tempo envia o que esta em timeoutBuffer
//}
