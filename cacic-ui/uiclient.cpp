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

    reconnectTimer = new QTimer(this);
    connect(reconnectTimer, SIGNAL(timeout()), this, SLOT(setupSocketConnection()));

    connected = false;
    canSend = false;
    setupSocketConnection();
}

QByteArray UiClient::formatData(QString message, int messageLength)
{
    QByteArray data;
    data.append(QString::number(messageLength));
    data.append(" ");
    data.append(message);
    data.append(MSG_END);

    return data;
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
    canSend = true;
    logcacic->escrever(LogCacic::InfoLevel,"Socket conectado.");

    if(reconnectTimer->isActive())
        reconnectTimer->stop();
}

void UiClient::on_disconnected()
{
    connected = false;
    logcacic->escrever(LogCacic::InfoLevel,"Socket desconectado.");

    reconnectTimer->start(3000);
}

void UiClient::on_forcarClicked()
{
    if(canSend) {
        if(connected) {
            QByteArray data = formatData(MSG_UIFORCAR, MSG_LENGTH_UIFORCAR);
            lastDataWritten = data;
            socket->write(data,data.size());
            socket->flush();
        } else
            logcacic->escrever(LogCacic::ErrorLevel,QString("forcarClicked: socket disconnected."));

        canSend = false;
    }

}

void UiClient::on_finalizarClicked()
{
    if(canSend) {
        if(connected) {
            QByteArray data = formatData(MSG_UIFINALIZAR,MSG_LENGTH_UIFINALIZAR);
            lastDataWritten = data;
            socket->write(data,data.size());
        } else {
            canSend = false;
            logcacic->escrever(LogCacic::ErrorLevel,QString("finalizarClicked: socket disconnected."));
            logcacic->escrever(LogCacic::ErrorLevel,"Serviço não respondeu ao Finalizar. Fechando UI.");
            emit finalizar();
        }
    } else {
        emit finalizar();
    }
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
                canSend = true;
                if(lastDataWritten == formatData(MSG_UIFINALIZAR,MSG_LENGTH_UIFINALIZAR))
                    emit finalizar();
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
        logcacic->escrever(LogCacic::ErrorLevel, "Erro ao conectar ao Cacic-service. Tentativa " + QString::number(connectionTrials));

        if(connectionTrials >= 3) {
            logcacic->escrever(LogCacic::ErrorLevel, "Não foi possível reestabelecer conexão com o Cacic-service. Finalizando Ui");
            emit finalizar();
        }
        connectionTrials++;
    }
}

