#include "serviceclient.h"

int const ServiceClient::CONNECTION_TIMEOUT = 1000;
int const ServiceClient::CONNECT_TIMER_TIMEOUT = 10000;
int const ServiceClient::MAX_CONNECTION_TRIALS = 3;
int const ServiceClient::MAX_MSGBUFFER_SIZE = 2;
int const ServiceClient::RECONNECT_TIMER_TIMEOUT = 3000;

ServiceClient* ServiceClient::pInstance = NULL;
WorkerThread* ServiceClient::workerThread = NULL;

ServiceClient::ServiceClient(const QString &dirpath, QObject *parent):
    QObject(parent)
{
    if(!dirpath.isEmpty()) {
        mainFolder = dirpath;
        logcacic = new LogCacic(LOG_DAEMON, mainFolder+"/Logs");
    } else {
        logcacic = new LogCacic(LOG_DAEMON, Identificadores::ENDERECO_PATCH_CACIC+"/Logs");
    }

    connectTimer = new QTimer(this);
    connect(connectTimer, SIGNAL(timeout()), this, SLOT(setupSocketConnection()));
    reconnectTimer = new QTimer(this);
    connect(reconnectTimer, SIGNAL(timeout()), this, SLOT(setupSocketConnection()));

    connected = false;
    canSend = false;

    setupSocketConnection();
}

QByteArray ServiceClient::formatData(QString message, int messageLength)
{
    QByteArray data;
    data.append(QString::number(messageLength));
    data.append(" ");
    data.append(message);
    data.append(MSG_END);

    return data;
}

ServiceClient* ServiceClient::Instance(const QString &dirpath, QObject *parent = 0)
{
    if(!pInstance) {
        pInstance = new ServiceClient(dirpath,parent);
        workerThread = new WorkerThread();
        pInstance->moveToThread(workerThread);
        workerThread->start();
    } else {
        if(!workerThread->isRunning())
            workerThread->start();
    }

    return pInstance;
}

bool ServiceClient::isConnected()
{
    return connected;
}

void ServiceClient::on_bytesWritten(qint64 bytes)
{

}

void ServiceClient::on_connected()
{
    connected = true;
    canSend = true;
    logcacic->escrever(LogCacic::InfoLevel,"Socket conectado.");

    if(reconnectTimer->isActive())
        reconnectTimer->stop();
}

void ServiceClient::on_disconnected()
{
    connected = false;
    logcacic->escrever(LogCacic::InfoLevel,"Socket desconectado.");

    reconnectTimer->start(RECONNECT_TIMER_TIMEOUT);
}

void ServiceClient::on_readyRead()
{
    QString dataReceived(socket->readLine());
    parseData(dataReceived);
}

void ServiceClient::on_sendStopUiMsg()
{
    if(canSend) {
        if(connected) {
            QByteArray data = formatData(MSG_DAEMONSTOPUI,MSG_LENGTH_DAEMONSTOPUI);
            lastDataWritten = data;
            socket->write(data,data.size());

            workerThread->run();
        } else {
            logcacic->escrever(LogCacic::ErrorLevel,"Não foi possível enviar mensagem para parar o CacicUi.");
        }
    } else {
        logcacic->escrever(LogCacic::ErrorLevel,"Não foi possível enviar mensagem para parar o CacicUi.");
    }

}

void ServiceClient::parseData(const QString &dataReceived)
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
                if(lastDataWritten == formatData(MSG_DAEMONSTOPUI,MSG_LENGTH_DAEMONSTOPUI)) {
                    emit uiStopped();
                }
            }
        }
    }
}

void ServiceClient::setupSocketConnection()
{
    socket = new QTcpSocket(this);

    connect(socket,&QTcpSocket::connected,this,&ServiceClient::on_connected);
    connect(socket,&QTcpSocket::disconnected,this,&ServiceClient::on_disconnected);
    connect(socket,&QTcpSocket::readyRead,this,&ServiceClient::on_readyRead);
    connect(socket,&QTcpSocket::bytesWritten,this,&ServiceClient::on_bytesWritten);

    socket->connectToHost("127.0.0.1",PORT_CACICUI);

    if(!socket->waitForConnected(CONNECTION_TIMEOUT)) {
        logcacic->escrever(LogCacic::ErrorLevel, "Erro ao conectar ao CacicUi. Tentativa " + QString::number(connectionTrials));

        if(connectionTrials == 0) {
            connectTimer->start(CONNECT_TIMER_TIMEOUT);
        } else if(connectionTrials >= MAX_CONNECTION_TRIALS) {
            logcacic->escrever(LogCacic::ErrorLevel, "Não foi possível estabelecer conexão com o Cacic-service. Finalizando Ui");

            if(connectTimer->isActive())
                connectTimer->stop();
            if(reconnectTimer->isActive())
                reconnectTimer->stop();

            emit finalizar();
        }
        connectionTrials++;
    }
}
