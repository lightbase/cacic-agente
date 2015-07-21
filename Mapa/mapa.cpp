#include "mapa.h"

Mapa::Mapa(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::Mapa)
{
    inicializarAtributos();
    preencheCampos();
}

Mapa::~Mapa()
{
    delete ui;
    delete oCacicComm;
    delete logcacic;

}

bool Mapa::checarPreenchimento() const
{
    if ( ui->lineNomeUsuario->text().isEmpty() ) {

        QMessageBox box(QMessageBox::Warning, "Formulário incompleto", "Há campo(s) não preenchido(s).", QMessageBox::Ok);
        box.setWindowFlags(Qt::Popup);
        if( box.exec() == QMessageBox::Ok ) {

            if(ui->lineNomeUsuario->text().isEmpty())
                ui->lineNomeUsuario->setFocus();

        }
        return false;
    } else {
        return true;
    }
}

void Mapa::closeEvent(QCloseEvent *event)
{
    QMessageBox box(QMessageBox::Warning, "Erro", "Não feche a janela. Preencha o formulário e pressione o botão de enviar.", QMessageBox::Ok);
    box.setWindowFlags(Qt::Popup);
    if( box.exec() == QMessageBox::Ok )
        event->ignore();
}

bool Mapa::enviarInfo(const QJsonObject &jsonMapa)
{
    bool ok = false;
    if (!jsonMapa.isEmpty()){
        QJsonObject retornoEnvio;
        logcacic->escrever(LogCacic::InfoLevel, QString("Enviando dados do Mapa ao gerente."));
        retornoEnvio = oCacicComm->comm(ROTA_COLETA, &ok, jsonMapa , true);
        if (!ok || retornoEnvio.contains("error") ||
            (retornoEnvio.contains("reply") && (retornoEnvio["reply"].isNull() || retornoEnvio.isEmpty()))){
            logcacic->escrever(LogCacic::ErrorLevel,  QString("Falha ao enviar dados do Mapa: " + retornoEnvio["error"].toString()));
        } else {

        }
//        QJsonObject coleta = CCacic::getJsonFromFile(this->mainFolder + "coleta.json");
//        coleta["mapa"] = jsonMapa["mapa"];
//        CCacic::setJsonToFile(coleta, this->mainFolder + "coleta.json");
    }

    return ok;
}

void Mapa::inicializarAtributos()
{
    QString folder = CCacic::getValueFromRegistry("Lightbase", "Cacic", "mainFolder").toString();
    mainFolder = !folder.isEmpty() && !folder.isNull() ? folder : Identificadores::ENDERECO_PATCH_CACIC;
    logcacic = new LogCacic(LOG_MAPA, mainFolder + "/Logs");

    oCacicComm = new CacicComm(LOG_MAPA, this->mainFolder);

    this->setWindowFlags(this->windowFlags() & ~Qt::WindowCloseButtonHint);
    this->setWindowFlags(this->windowFlags() | Qt::X11BypassWindowManagerHint | Qt::WindowStaysOnTopHint);

    this->setWindowState(this->windowState() | Qt::WindowFullScreen | Qt::WindowActive);

    QDesktopWidget desktopWidget;
    QRect screenSize = desktopWidget.frameGeometry();

    this->setMinimumSize(screenSize.size());

    keyPressEater = new KeyPressEater();

    this->installEventFilter(keyPressEater);
    this->setFocusPolicy(Qt::ClickFocus);

    ui->setupUi(this);
}

void Mapa::on_okButton_clicked()
{

    if ( checarPreenchimento() ) {
        QList<QPair<QString,QString> > listaValores;
        if( validarCampos(listaValores) ) {
            this->hide();
            QJsonObject jsonMapa;
            jsonMapa["computador"] = computer.toJsonObject();

            QJsonObject patrimonio;
            QPair<QString,QString> linePair;
            foreach( linePair, listaValores)
                patrimonio[linePair.first] = QJsonValue::fromVariant(linePair.second);

            jsonMapa["mapa"] = patrimonio;
            QMessageBox *box;
            if (enviarInfo(jsonMapa)){
                box = new QMessageBox(QMessageBox::Information, "Sucesso!", "Informações obtidas com sucesso.", QMessageBox::Ok);
            } else {
                box = new QMessageBox(QMessageBox::Warning, "Erro!",
                                "Falha ao enviar as informações.\nUma nova tentativa será realizada posteriormente.",
                                QMessageBox::Ok);
            }
            box->setWindowFlags(Qt::Popup);

            if( box->exec() == QMessageBox::Ok )
//                emit finished();
                exit(0);
        }
    }
}

void Mapa::preencheCampos()
{
    QJsonObject computerJson = computer.toJsonObject();

    ui->lineIdUsuario->setText(computerJson["usuario"].toString());
    ui->lineNomeComputador->setText(computerJson["nmComputador"].toString());
    ui->lineEnderecoIp->setText(computerJson["networkDevices"].toArray().first().toObject()["ipv4"].toString());

    ui->lineIdUsuario->setDisabled(true);
    ui->lineNomeComputador->setDisabled(true);
    ui->lineEnderecoIp->setDisabled(true);

    QString nome = preencheNomeUsuario();
    if (!nome.isNull() && !nome.isEmpty()){
        ui->lineNomeUsuario->setText(nome);
        ui->lineNomeUsuario->setDisabled(true);
    }
}

QString Mapa::preencheNomeUsuario()
{
    QString ldapServer, ldapLogin, ldapPass, ldapBase, ldapFilter;
    QJsonObject configs = CCacic::getJsonFromFile(this->mainFolder + "getConfig.json");

    if (!configs.isEmpty() && !configs["agentcomputer"].isNull()){
        QJsonObject ldap = configs["agentcomputer"].toObject()["ldap"].toObject();
        if(!ldap.isEmpty()) {
            ldapBase = ldap["base"].toString();

            //ARRUMAR ESSA LINHA DEPOIS
            ldapFilter = ldap["filter"].toArray().first().toString();

            ldapLogin = ldap["login"].toString();
            ldapPass = ldap["pass"].toString();
            ldapServer = ldap["server"].toString();
        } else {
            logcacic->escrever(LogCacic::InfoLevel, "Não há dados de configurações LDAP, usuário deverá digitar o nome.");
            QMessageBox box(QMessageBox::Information, "Atenção!", "Não foi possível recuperar o seu nome, por gentileza insira-o.", QMessageBox::Ok);
            box.setWindowFlags(Qt::Popup);

            if( box.exec() == QMessageBox::Ok )
//                emit finished();
            return QString();
        }
    } else {
        logcacic->escrever(LogCacic::ErrorLevel, "Não foi possível pegar informações do arquivo de configurações.");
        return QString();
    }

//    ldapBase = "ou=usuarios,dc=lightbase,dc=com,dc=br";
//    ldapFilter = "(&(objectClass=*)(uid=thiagop))";
//    ldapLogin = "cn=System Administrator-gosa-admin,ou=usuarios,dc=lightbase,dc=com,dc=br";
//    ldapPass = "brlight2012";
//    ldapServer = "ldap.lightbase";
    LdapHandler ldapHandler(ldapServer);

    if ( ldapHandler.inicializar() )
        return ldapHandler.busca(ldapLogin,ldapPass,ldapBase,ldapFilter);

    return QString();
}

void Mapa::setComm(const QString &server)
{
    oCacicComm->setUrlGerente(server);

    if( !CCacic::getValueFromRegistry("Lightbase", "Cacic", "applicationUrl").isNull() ) {
        oCacicComm->setUsuario(CCacic::getValueFromRegistry("Lightbase", "Cacic", "usuario").toString());
        oCacicComm->setPassword(CCacic::getValueFromRegistry("Lightbase", "Cacic", "password").toString());
    }
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
        box.setWindowFlags(Qt::Popup);
        if( box.exec() == QMessageBox::Ok )
            return false;
    } else if( !regAlfa.exactMatch(nomeUsuario) ) {
        ui->lineNomeUsuario->setFocus();

        QMessageBox box(QMessageBox::Warning, "Preenchimento inválido.", "Campo de Nome deve conter apenas letras.", QMessageBox::Ok);
        box.setWindowFlags(Qt::Popup);
        if( box.exec() == QMessageBox::Ok )
            return false;
    } else if( !coordenacao.isEmpty() && coordenacao.size() < 2) {
        ui->lineCoordenacao->setFocus();

        QMessageBox box(QMessageBox::Warning, "Preenchimento inválido.", "Campo de Coordenação deve ter ao menos 2 caracteres.", QMessageBox::Ok);
        box.setWindowFlags(Qt::Popup);
        if( box.exec() == QMessageBox::Ok )
            return false;
    } else if( !sala.isEmpty() && sala.toInt() == 0) {
        ui->lineSala->setFocus();

        QMessageBox box(QMessageBox::Warning, "Preenchimento inválido.", "Campo de Sala deve conter dígitos numéricos diferentes de 0.", QMessageBox::Ok);
        box.setWindowFlags(Qt::Popup);
        if( box.exec() == QMessageBox::Ok )
            return false;
    } else if( !patrimonioComputador.isEmpty() && patrimonioComputador.toInt() == 0) {
        ui->linePatrimonioComputador->setFocus();

        QMessageBox box(QMessageBox::Warning, "Preenchimento inválido.", "Campo de Patrimônio Computador deve conter dígitos numéricos diferentes de 0.", QMessageBox::Ok);
        box.setWindowFlags(Qt::Popup);
        if( box.exec() == QMessageBox::Ok )
            return false;
    } else if( !patrimonioMonitor1.isEmpty() && patrimonioMonitor1.toInt() == 0) {
        ui->linePatrimonioMonitor1->setFocus();

        QMessageBox box(QMessageBox::Warning, "Preenchimento inválido.", "Campo de Patrimônio Monitor 1 deve conter dígitos numéricos diferentes de 0.", QMessageBox::Ok);
        box.setWindowFlags(Qt::Popup);
        if( box.exec() == QMessageBox::Ok )
            return false;
    } else if( !patrimonioMonitor2.isEmpty() && patrimonioMonitor2.toInt() == 0) {
        ui->linePatrimonioMonitor2->setFocus();

        QMessageBox box(QMessageBox::Warning, "Preenchimento inválido.", "Campo de Patrimônio Monitor 2 deve conter dígitos numéricos diferentes de 0.", QMessageBox::Ok);
        box.setWindowFlags(Qt::Popup);
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
