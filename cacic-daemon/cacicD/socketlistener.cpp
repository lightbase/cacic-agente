#include "socketlistener.h"

SocketListener::SocketListener(QString DirPath, QObject *parent) :
    QObject(parent)
{
    this->applicationDirPath = DirPath;
    server = new QTcpServer(this);
    connect(server, SIGNAL(newConnection()), this, SLOT(newConnection()));
    iniciarInstancias();
}

void SocketListener::newConnection() {
    QTcpSocket *socket = server->nextPendingConnection();
    socket->write("Pedido para coleta forçada recebido com sucesso.");
    socket->flush();
    socket->waitForBytesWritten(3000);
    socket->close();
    iniciarColetaForcada();
}

void SocketListener::setPort_no(int value) {
    port_no = value;
}

SocketListener::~SocketListener(){
    server->close();
}

void SocketListener::iniciarInstancias() {
    logManager = QLogger::QLoggerManager::getInstance();
    logManager->addDestination(this->applicationDirPath + "/Logs/cacic.log",Identificadores::LOG_SOCKET_LISTENER, QLogger::InfoLevel);
    logManager->addDestination(this->applicationDirPath + "/Logs/cacic_error.log",Identificadores::LOG_SOCKET_LISTENER, QLogger::ErrorLevel);
    if(!server->listen(QHostAddress::Any, this->port_no)) {
        QLogger::QLog_Info(Identificadores::LOG_SOCKET_LISTENER, QString("Erro ao iniciar escuta de socket."));
    } else {
        QLogger::QLog_Info(Identificadores::LOG_SOCKET_LISTENER, QString("Escuta na porta " + QString::number(this->port_no) + " iniciada com sucesso."));
    }
}

void SocketListener::iniciarColetaForcada() {
    cacicthread = new CacicThread(this->applicationDirPath);
    ccacic = new CCacic();
    oCacicComm = new CacicComm();
    cMutex = new QMutex();
    cacicthread->setCcacic(ccacic);
    cacicthread->setOCacicComm(oCacicComm);
    cacicthread->setNomeModulo("gercols.exe");
    cacicthread->setCMutex(cMutex);
    cacicthread->setModuloDirPath(this->applicationDirPath+"/gercols.exe");
    cacicthread->start(QThread::NormalPriority);
}
