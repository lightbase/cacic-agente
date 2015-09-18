#include "cacicwidget.h"
#include "ui_widget.h"

CacicWidget::CacicWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Widget)
{
    QString folder = CCacic::getValueFromRegistry("Lightbase", "Cacic", "mainFolder").toString();
    cacicMainFolder = !folder.isEmpty() && !folder.isNull() ? folder : Identificadores::ENDERECO_PATCH_CACIC;

    logcacic = new LogCacic(LOG_CACICUI, cacicMainFolder+"/Logs");

    connected = false;

    setupSocketConnection();
    cacicSysTray = new CacicSysTray(cacicMainFolder,socket,this);
    connect(cacicSysTray,&CacicSysTray::infosClicked,this,&CacicWidget::setupWidget);
    cacicSysTray->iniciarSysTray();

    ui->setupUi(this);
}

CacicWidget::~CacicWidget()
{
    delete ui;
    delete cacicSysTray;
}

bool CacicWidget::isConnected()
{
    return connected;
}

void CacicWidget::setupSocketConnection()
{
    socket = new QTcpSocket(this);

    connect(socket,&QTcpSocket::connected,this,&CacicWidget::socketConnected);
    connect(socket,&QTcpSocket::disconnected,this,&CacicWidget::socketDisconnected);
    connect(socket,&QTcpSocket::readyRead,this,&CacicWidget::socketReadyRead);
    connect(socket,&QTcpSocket::bytesWritten,this,&CacicWidget::socketBytesWritten);

    socket->connectToHost("127.0.0.1",CACICDAEMON_PORT);

    if(!socket->waitForConnected(1000)) {
        logcacic->escrever(LogCacic::ErrorLevel, "Erro ao conectar ao Cacic-service.");
    }

}

void CacicWidget::socketBytesWritten(qint64 bytes)
{

}

void CacicWidget::socketConnected()
{
    connected = true;
    logcacic->escrever(LogCacic::InfoLevel,"Socket conectado.");
}

void CacicWidget::socketDisconnected()
{
    connected = false;
    logcacic->escrever(LogCacic::InfoLevel,"Socket desconectado.");
}

void CacicWidget::socketReadyRead()
{
    logcacic->escrever(LogCacic::InfoLevel,"Socket recebeu mensagem:");
    QString mensagem(socket->readAll());
    logcacic->escrever(LogCacic::InfoLevel,mensagem);

}

void CacicWidget::setupWidget()
{
    logcacic->escrever(LogCacic::InfoLevel,"SetupWidget");
}
