#include "cacicmessage.h"
#include "ui_cacicmessage.h"

CacicMessage::CacicMessage(QStringList args, QWidget *parent) : QMainWindow(parent), ui(new Ui::CacicMessage)
{
    ui->setupUi(this);
    logcacic = new LogCacic(Identificadores::LOG_CACIC_MESSAGE, Identificadores::ENDERECO_PATCH_CACIC + "/Logs");
    validaArgumentos(args);
}

CacicMessage::~CacicMessage()
{
    delete ui;
}

void CacicMessage::validaArgumentos(QStringList args){
    if(args.empty()) return;
    foreach (QString var, args) {
        if(var.contains("-titulo=")) setTitulo(var.split("=").last());
        if(var.contains("-mensagem=")) setMensagem(var.split("=").last());
        if(var.contains("-hash=")) setHash(var.split("=").last());
    }
}

void CacicMessage::exibeMensagem(){
#ifdef Q_OS_WIN
    QMessageBox box;
    box.setWindowTitle("Cacic Messages");
    box.setText(getTitulo());
#elif Q_OS_LINUX
    QMessageBox box(QMessageBox::Information, getTitulo(), getMensagem(), QMessageBox::Ok);
    box.setWindowFlags(box.windowFlags() & ~Qt::WindowCloseButtonHint);
#endif
    box.setDetailedText(getMensagem()); //caso queria enviar mensagens maiores e mais detalhadas
    if (QMessageBox::Ok == box.exec()){
        logcacic->escrever(LogCacic::MessageControler,
                           QString("hash=" + getHash() + " ; ok=" + QDateTime::currentDateTime().toLocalTime().toString()));
        emit finished();
    }
}

QString CacicMessage::getHash() const
{
    return hash;
}

void CacicMessage::setHash(const QString &value)
{
    hash = value;
}

QString CacicMessage::getMensagem() const
{
    return mensagem;
}

void CacicMessage::setMensagem(const QString &value)
{
    mensagem = value;
}

QString CacicMessage::getTitulo() const
{
    return titulo;
}

void CacicMessage::setTitulo(const QString &value)
{
    titulo = value;
}
