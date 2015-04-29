#include "mapa.h"


Mapa::Mapa(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::Mapa)
{

    QString folder = CCacic::getValueFromRegistry("Lightbase", "Cacic", "mainFolder").toString();
    mainFolder = !folder.isEmpty() && !folder.isNull() ? folder : Identificadores::ENDERECO_PATCH_CACIC;
    logcacic = new LogCacic(LOG_MAPA, mainFolder + "/Logs");

    this->setWindowFlags(this->windowFlags() | Qt::WindowStaysOnTopHint);
    this->setWindowFlags(this->windowFlags() & ~Qt::WindowCloseButtonHint);
    this->setWindowState(this->windowState() | Qt::WindowFullScreen | Qt::WindowActive );

#if defined(Q_OS_WIN)
    QDesktopWidget desktopWidget;
    QRect screenSize = desktopWidget.frameGeometry();

    this->setMinimumSize(screenSize.size());
#endif

    ui->setupUi(this);

    preencheCampos(true, "ldap://lightbase.com.br:389");
}

Mapa::~Mapa()
{
    delete logcacic;
    delete ui;
}

bool Mapa::checarPreenchimento() const
{
    if ( ui->lineNomeUsuario->text().isEmpty() ) {

        QMessageBox box(QMessageBox::Warning, "Formulário incompleto", "Há campo(s) não preenchido(s).", QMessageBox::Ok);
        box.setWindowFlags(Qt::WindowStaysOnTopHint);
        if( box.exec() == QMessageBox::Ok ) {

            if(ui->lineNomeUsuario->text().isEmpty())
                ui->lineNomeUsuario->setFocus();

            return false;
        }
    } else {
        return true;
    }
}

void Mapa::closeEvent(QCloseEvent *event)
{
    QMessageBox box(QMessageBox::Warning, "Erro", "Não feche a janela. Preencha o formulário e pressione o botão de enviar.", QMessageBox::Ok);
    box.setWindowFlags(Qt::WindowStaysOnTopHint);
    if( box.exec() == QMessageBox::Ok )
        event->ignore();
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
    box.setWindowFlags(Qt::WindowStaysOnTopHint);
    if( box.exec() == QMessageBox::Ok )
        qApp->quit();
}

void Mapa::on_okButton_clicked()
{

    if ( checarPreenchimento() ) {
        QList<QPair<QString,QString> > listaValores;
        if( validarCampos(listaValores) ) {

            QJsonObject jsonMapa;
            jsonMapa["computador"] = computer.toJsonObject();

            QPair<QString,QString> linePair;
            foreach( linePair, listaValores)
                jsonMapa[linePair.first] = QJsonValue::fromVariant(linePair.second);

            enviarInfo(jsonMapa);
        }
    }
}

void Mapa::preencheCampos(bool preencherUsuario, const QString &ldapUrl)
{
    QJsonObject computerJson = computer.toJsonObject();

    ui->lineIdUsuario->setText(computerJson["usuario"].toString());
    ui->lineNomeComputador->setText(computerJson["nmComputador"].toString());
    ui->lineEnderecoIp->setText(computerJson["networkDevices"].toArray().last().toObject()["ipv4"].toString());

    ui->lineIdUsuario->setDisabled(true);
    ui->lineNomeComputador->setDisabled(true);
    ui->lineEnderecoIp->setDisabled(true);

    if( preencherUsuario )
        preencheNomeUsuario(ldapUrl);
}

bool Mapa::preencheNomeUsuario(const QString &ldapUrl)
{

    LDAP *ldp;
    LDAPMessage **res;
    int rc;

    rc = ldap_initialize( &ldp, ldapUrl.toStdString().c_str());
    if ( rc != LDAP_SUCCESS){
        return false;
    }

//    ldap_set_option(ldp, LDAP_OPT_PROTOCOL_VERSION, (void*)3);

//    char **attrs = (char**)LDAP_ALL_USER_ATTRIBUTES;
//    rc = ldap_search_ext_s(ldp,"ou=usuarios,dc=lightbase,dc=com,dc=br",LDAP_SCOPE_BASE,NULL,attrs,0,NULL,NULL,NULL,0,res);
//    if ( rc != LDAP_SUCCESS){
//        QMessageBox box(QMessageBox::Warning, "LDAP Search error.", "Function ldap_search_ext_s behaved badly.", QMessageBox::Ok);
//        box.setWindowFlags(Qt::WindowStaysOnTopHint);
//        if( box.exec() == QMessageBox::Ok )
//            return false;
//    }

//    ldap_sasl_bind(ldp,"ou=usuarios,dc=lightbase,dc=com,dc=br",);
    //ldap_search_ext_s

    return true;
}

bool Mapa::setArgs(int argc, char *argv[])
{
    return true;
}

bool Mapa::validarCampos(QList<QPair<QString,QString> > &listaValores)
{
    QString nomeUsuario = ui->lineNomeUsuario->text();
    QString coordenacao = ui->lineCoordenacao->text();
    QString sala = ui->lineSala->text();
    QString patrimonioComputador = ui->linePatrimonioComputador->text();
    QString patrimonioMonitor1 = ui->linePatrimonioMonitor1->text();
    QString patrimonioMonitor2 = ui->linePatrimonioMonitor2->text();

    QRegExp regAlfa("^([\ \'\.\^\~\´\`\\áÁ\\àÀ\\ãÃ\\âÂ\\éÉ\\èÈ\\êÊ\\íÍ\\ìÌ\\óÓ\\òÒ\\õÕ\\ôÔ\\úÚ\\ùÙ\\çÇaA-zZ]+)+((\s[\'\.\^\~\´\`\\áÁ\\àÀ\\ãÃ\\âÂ\\éÉ\\èÈ\\êÊ\\íÍ\\ìÌ\\óÓ\\òÒ\\õÕ\\ôÔ\\úÚ\\ùÙ\\çÇaA-zZ]+)+)?$");
    QRegExp regNum("^[0-9]$");

    if( nomeUsuario.size() <= 2 ) {
        ui->lineNomeUsuario->setFocus();

        QMessageBox box(QMessageBox::Warning, "Preenchimento inválido.", "Campo de Nome deve ao menos 3 caracteres.", QMessageBox::Ok);
        box.setWindowFlags(Qt::WindowStaysOnTopHint);
        if( box.exec() == QMessageBox::Ok )
            return false;
    } else if( !regAlfa.exactMatch(nomeUsuario) ) {
        ui->lineNomeUsuario->setFocus();

        QMessageBox box(QMessageBox::Warning, "Preenchimento inválido.", "Campo de Nome deve conter apenas letras.", QMessageBox::Ok);
        box.setWindowFlags(Qt::WindowStaysOnTopHint);
        if( box.exec() == QMessageBox::Ok )
            return false;
    } else if( !coordenacao.isEmpty() && coordenacao.size() < 2) {
        ui->lineCoordenacao->setFocus();

        QMessageBox box(QMessageBox::Warning, "Preenchimento inválido.", "Campo de Coordenação deve ter ao menos 2 caracteres.", QMessageBox::Ok);
        box.setWindowFlags(Qt::WindowStaysOnTopHint);
        if( box.exec() == QMessageBox::Ok )
            return false;
    } else if( !sala.isEmpty() && sala.toInt() == 0) {
        ui->lineSala->setFocus();

        QMessageBox box(QMessageBox::Warning, "Preenchimento inválido.", "Campo de Sala deve conter dígitos numéricos diferentes de 0.", QMessageBox::Ok);
        box.setWindowFlags(Qt::WindowStaysOnTopHint);
        if( box.exec() == QMessageBox::Ok )
            return false;
    } else if( !patrimonioComputador.isEmpty() && patrimonioComputador.toInt() == 0) {
        ui->linePatrimonioComputador->setFocus();

        QMessageBox box(QMessageBox::Warning, "Preenchimento inválido.", "Campo de Patrimônio Computador deve conter dígitos numéricos diferentes de 0.", QMessageBox::Ok);
        box.setWindowFlags(Qt::WindowStaysOnTopHint);
        if( box.exec() == QMessageBox::Ok )
            return false;
    } else if( !patrimonioMonitor1.isEmpty() && patrimonioMonitor1.toInt() == 0) {
        ui->linePatrimonioMonitor1->setFocus();

        QMessageBox box(QMessageBox::Warning, "Preenchimento inválido.", "Campo de Patrimônio Monitor 1 deve conter dígitos numéricos diferentes de 0.", QMessageBox::Ok);
        box.setWindowFlags(Qt::WindowStaysOnTopHint);
        if( box.exec() == QMessageBox::Ok )
            return false;
    } else if( !patrimonioMonitor2.isEmpty() && patrimonioMonitor2.toInt() == 0) {
        ui->linePatrimonioMonitor2->setFocus();

        QMessageBox box(QMessageBox::Warning, "Preenchimento inválido.", "Campo de Patrimônio Monitor 2 deve conter dígitos numéricos diferentes de 0.", QMessageBox::Ok);
        box.setWindowFlags(Qt::WindowStaysOnTopHint);
        if( box.exec() == QMessageBox::Ok )
            return false;
    }

    listaValores.append( QPair<QString,QString>("nomeUsuario",nomeUsuario));
    if( !coordenacao.isEmpty() ) listaValores.append( QPair<QString,QString>("coordenacao",coordenacao));
    if( !sala.isEmpty() ) listaValores.append( QPair<QString,QString>("sala",sala));
    if( !patrimonioComputador.isEmpty() ) listaValores.append( QPair<QString,QString>("patrimonioComputador",patrimonioComputador));
    if( !patrimonioMonitor1.isEmpty() ) listaValores.append( QPair<QString,QString>("patrimonioMonitor1",patrimonioMonitor1));
    if( !patrimonioMonitor2.isEmpty() ) listaValores.append( QPair<QString,QString>("patrimonioMonitor2",patrimonioMonitor2));

    return true;
}
