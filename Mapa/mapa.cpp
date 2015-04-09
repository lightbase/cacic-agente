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
    this->setWindowState(this->windowState() | Qt::WindowFullScreen | Qt::WindowActive );

    ui->setupUi(this);

//    ui->lineIdUsuario;
//    ui->lineNomeComputador;
//    ui->lineEnderecoIp;
    QJsonObject computerJson = computer.toJsonObject();

    ui->lineIdUsuario->setText(computerJson["usuario"].toString());
    ui->lineNomeComputador->setText(computerJson["nmComputador"].toString());
    ui->lineEnderecoIp->setText(computerJson["networkDevices"].toArray().last().toObject()["ipv4"].toString());

    ui->lineIdUsuario->setDisabled(true);
    ui->lineNomeComputador->setDisabled(true);
    ui->lineEnderecoIp->setDisabled(true);
}

Mapa::~Mapa()
{
    delete logcacic;
    delete ui;
}

bool Mapa::checarPreenchimento() const
{
    if ( ui->lineNomeUsuario->text().isEmpty() ||
         ui->lineCoordenacao->text().isEmpty() ||
         ui->lineSala->text().isEmpty() ||
         ui->linePatrimonioComputador->text().isEmpty() ||
         ui->linePatrimonioMonitor1->text().isEmpty() ||
         ui->linePatrimonioMonitor2->text().isEmpty() ) {

        QMessageBox box(QMessageBox::Warning, "Formulário incompleto", "Há campo(s) não preenchido(s).", QMessageBox::Ok);
        if( box.exec() == QMessageBox::Ok )
            return false;
    } else {
        return true;
    }
}

bool Mapa::checarCampos()
{

    return true;
}

bool Mapa::enviarInfo(const QJsonObject &jsonMapa)
{
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

    QMessageBox box(QMessageBox::Information, "Sucesso!", "Informações obtidas com sucesso.", QMessageBox::Ok);
    if( box.exec() == QMessageBox::Ok )
        qApp->quit();
}

void Mapa::on_okButton_clicked()
{

    if ( checarPreenchimento() ) {
        if( checarCampos() ) {

            QString patrimonioComputador = ui->linePatrimonioComputador->text();
            QString patrimonioMonitor = ui->linePatrimonioMonitor1->text();

            QJsonObject jsonMapa;
            jsonMapa["computador"] = computer.toJsonObject();
            jsonMapa["PatrimonioComputador"] = QJsonValue::fromVariant(patrimonioComputador);
            jsonMapa["PatrimonioMonitor"] = QJsonValue::fromVariant(patrimonioMonitor);

            enviarInfo(jsonMapa);
        }
    }
}
