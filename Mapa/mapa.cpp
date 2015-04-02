#include "mapa.h"

Mapa::Mapa(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::Mapa)
{

    QString folder = CCacic::getValueFromRegistry("Lightbase", "Cacic", "mainFolder").toString();
    mainFolder = !folder.isEmpty() && !folder.isNull() ? folder : Identificadores::ENDERECO_PATCH_CACIC;
    logcacic = new LogCacic(LOG_MAPA, mainFolder + "/Logs");

    this->setWindowFlags(this->windowFlags() & Qt::BypassWindowManagerHint);
    this->setWindowFlags(this->windowFlags() & Qt::WindowStaysOnTopHint);
    this->setWindowFlags(this->windowFlags() & ~Qt::WindowCloseButtonHint);
    this->setWindowState(this->windowState() | Qt::WindowFullScreen);

    ui->setupUi(this);
}

Mapa::~Mapa()
{
    delete logcacic;
    delete ui;
}

void Mapa::on_okButton_clicked()
{

    QString patrimonioComputador = ui->editPatrimonioComputador->text();
    QString patrimonioMonitor = ui->editPatrimonioMonitor->text();

    QJsonObject jsonMapa;
    jsonMapa["PatrimonioComputador"] = QJsonValue::fromVariant(patrimonioComputador);
    jsonMapa["PatrimonioMonitor"] = QJsonValue::fromVariant(patrimonioMonitor);

    bool ok = false;
    if (!jsonMapa.isEmpty()){
        CacicComm *OCacicComm = new CacicComm(LOG_MAPA, this->mainFolder);
        OCacicComm->setUrlGerente(CCacic::getValueFromRegistry("Lightbase", "Cacic", "applicationUrl").toString());
        OCacicComm->setUsuario(CCacic::getValueFromRegistry("Lightbase", "Cacic", "usuario").toString());
        OCacicComm->setPassword(CCacic::getValueFromRegistry("Lightbase", "Cacic", "password").toString());
        QJsonObject retornoEnvio;
        logcacic->escrever(LogCacic::InfoLevel, QString("Enviando dados do Mapa ao gerente."));
        retornoEnvio = OCacicComm->comm(ROTA_MAPA, &ok, jsonMapa , true);
        if(retornoEnvio.contains("error")) {
            logcacic->escrever(LogCacic::ErrorLevel,  QString("Falha ao enviar a dados do Mapa: " + retornoEnvio["error"].toString()));
        }
    }

    qApp->quit();
}
