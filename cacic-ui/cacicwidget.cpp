#include "cacicwidget.h"
#include "ui_widget.h"

CacicWidget::CacicWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Widget)
{
    QString folder = CCacic::getValueFromRegistry("Lightbase", "Cacic", "mainFolder").toString();
    cacicMainFolder = !folder.isEmpty() && !folder.isNull() ? folder : Identificadores::ENDERECO_PATCH_CACIC;

    logcacic = new LogCacic(LOG_CACICUI, cacicMainFolder+"/Logs");

    windowOpen = false;

    cliente = new UiClient(cacicMainFolder,this);
    cacicSysTray = new CacicSysTray(cacicMainFolder,this);

    connect(cacicSysTray,&CacicSysTray::infosClicked,this,&CacicWidget::on_infosClicked,Qt::UniqueConnection);
    connect(cacicSysTray,&CacicSysTray::forcarClicked,cliente,&UiClient::on_forcarClicked,Qt::UniqueConnection);
    connect(cacicSysTray,&CacicSysTray::finalizarClicked,cliente,&UiClient::on_finalizarClicked,Qt::UniqueConnection);
    connect(cliente,&UiClient::finalizar,this,&CacicWidget::on_finalizar,Qt::QueuedConnection);

    cacicSysTray->iniciarSysTray();

    ui->setupUi(this);
}

CacicWidget::~CacicWidget()
{
    delete ui;
    delete cacicSysTray;
}

void CacicWidget::closeEvent(QCloseEvent *event)
{
        event->ignore();
        this->hide();
        windowOpen = false;
}

void CacicWidget::on_finalizar()
{
    exit(0);
}

void CacicWidget::on_infosClicked()
{
    if(!windowOpen) {
        windowOpen = true;
        logcacic->escrever(LogCacic::InfoLevel,"SetupWidget here");

        QJsonObject coleta = CCacic::getJsonFromFile(cacicMainFolder + "/coleta.json");

        if(!coleta.isEmpty()) {
            setupTabGeral(coleta);
            setupTabHardware(coleta);
            setupTabSoftware(coleta);
        }

        this->show();
    }
}

void CacicWidget::setupTabGeral(const QJsonObject &coleta)
{
    if( coleta.contains("computador") && coleta["computador"].isObject() ) {
        QJsonObject computador = coleta["computador"].toObject();

        if( computador.contains("nmComputador") )
            ui->lineNomeComputador->setText(computador["nmComputador"].toString());
        if( computador.contains("usuario") )
            ui->lineUsuario->setText(computador["usuario"].toString());
        if( computador.contains("versaoAgente") )
            ui->lineVersaoAgente->setText(computador["versaoAgente"].toString());
        if( computador.contains("versaoGercols") )
            ui->lineVersaoGercols->setText(computador["versaoGercols"].toString());

        if( computador.contains("operatingSystem") && computador["operatingSystem"].isObject() ) {
            QJsonObject osJson = computador["operatingSystem"].toObject();

            if( osJson.contains("idOs") )
                ui->lineIdOs->setText(QString::number(osJson["idOs"].toInt()));
            if( osJson.contains("nomeOs") )
                ui->lineNomeOs->setText(osJson["nomeOs"].toString());
            if( osJson.contains("tipo") )
                ui->lineTipoOs->setText(osJson["tipo"].toString());
            if( osJson.contains("upTime") )
                ui->lineUptime->setText(QString::number(osJson["upTime"].toInt()));
        }

        if( computador.contains("networkDevices") && computador["networkDevices"].isArray() ) {
            if( !computador["networkDevices"].toArray().isEmpty() ) {
                QJsonArray networkDevices = computador["networkDevices"].toArray();

                for(int i = 0; i < networkDevices.size(); i++) {
                    if(networkDevices.at(i).isObject() && !networkDevices.at(i).toObject().isEmpty()) {
                        QJsonObject netDev = networkDevices.at(i).toObject();

                        NetDevTab *newNetTab = new NetDevTab(netDev,ui->tabNetDevs);

                        ui->tabNetDevs->addTab(newNetTab,"Dispositivo" + QString::number(i+1));
                        ui->tabNetDevs->setCurrentIndex(0);
                    }
                }
            }
        }
    }
}

void CacicWidget::setupTabHardware(const QJsonObject &coleta)
{

}

void CacicWidget::setupTabSoftware(const QJsonObject &coleta)
{

}
