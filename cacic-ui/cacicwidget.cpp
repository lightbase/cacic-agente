#include "cacicwidget.h"
#include "ui_widget.h"

CacicWidget::CacicWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Widget)
{
    QString folder = CCacic::getValueFromRegistry("Lightbase", "Cacic", "mainFolder").toString();
    cacicMainFolder = !folder.isEmpty() && !folder.isNull() ? folder : Identificadores::ENDERECO_PATCH_CACIC;

    logcacic = new LogCacic(LOG_CACICUI, cacicMainFolder+"/Logs");

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
}

void CacicWidget::on_finalizar()
{
    exit(0);
}

void CacicWidget::on_infosClicked()
{
    logcacic->escrever(LogCacic::InfoLevel,"SetupWidget here");

    QJsonObject coleta = CCacic::getJsonFromFile(cacicMainFolder + "/coleta.json");

    if(!coleta.isEmpty()) {
        setupTabGeral(coleta);
        setupTabHardware(coleta);
        setupTabSoftware(coleta);
    }

    this->show();
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

    }
}

void CacicWidget::setupTabHardware(const QJsonObject &coleta)
{

}

void CacicWidget::setupTabSoftware(const QJsonObject &coleta)
{

}
