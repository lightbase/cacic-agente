#include "mapa.h"

/**
 * @brief Mapa::Mapa
 * @param parent
 */
Mapa::Mapa(const QJsonObject &getMapaJson, QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::Mapa)
{
    inicializarAtributos(getMapaJson);
    preencheCampos();
}

/**
 * @brief Mapa::~Mapa
 */
Mapa::~Mapa()
{
    delete ui;
    delete logcacic;
}

/**
 * @brief Mapa::checarPreenchimento
 * @return bool com true se o form tiver preenchido
 *
 * Checa se os campos obrigatórios do formulário estão preenchidos.
 */
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

/**
 * @brief Mapa::closeEvent
 * @param event
 *
 * Lida com o evento de fechamento de janela, evitando que o usuário
 * feche o Mapa sem ter feito as operações necessárias.
 */
void Mapa::closeEvent(QCloseEvent *event)
{
    QMessageBox box(QMessageBox::Warning, "Erro", "Não feche a janela. Preencha o formulário e pressione o botão de enviar.", QMessageBox::Ok);
    box.setWindowFlags(Qt::Popup);
    if( box.exec() == QMessageBox::Ok )
        event->ignore();
}

/**
 * @brief Mapa::geraCampoMensagem
 */
void Mapa::geraCampoMensagem()
{
    if (mapaJson.contains("message") &&
            !mapaJson["message"].isNull() &&
            mapaJson["message"].isString() &&
            !mapaJson["message"].toString().isEmpty()) {

        QString strMessage = mapaJson["message"].toString();
        ui->labelMessage->setText(strMessage);
    }
}

/**
 * @brief Mapa::geraNovosCampos
 *
 * Gera novos campos para inserção de texto no formulário a partir
 * do que é passado pelo gerente no campo "properties" do getMapa.
 */
void Mapa::geraNovosCampos()
{
    if (mapaJson.contains("properties") &&
            !mapaJson["properties"].isNull() &&
            !mapaJson["properties"].toObject().isEmpty()) {
        QJsonObject campos = mapaJson["properties"].toObject();

        foreach(QString keyJson, campos.keys()) {
            NovoCampo novo;

            QString strLabel = campos[keyJson].toObject()["name"].isString() ?
                        campos[keyJson].toObject()["name"].toString() :
                        "";
            QString strDescription = campos[keyJson].toObject()["description"].isString() ?
                        campos[keyJson].toObject()["description"].toString() :
                        "";
            novo.setTitle(keyJson);
            novo.setLabel(strLabel);
            novo.setDescription(strDescription);

            novo.setLabelWidget(new QLabel(novo.getLabel()));
            novo.setLineWidget(new QLineEdit(novo.getDescription()));

            if( ui->formLayout_3->rowCount() <= ui->formLayout_4->rowCount())
                ui->formLayout_3->addRow(novo.getLabelWidget(),novo.getLineWidget());
            else
                ui->formLayout_4->addRow(novo.getLabelWidget(),novo.getLineWidget());

            listNovosCampos.push_back(novo);
        }
    }
}

/**
 * @brief Mapa::inicializarAtributos
 *
 * Inicializa atributos e objetos necessários, e seta o modo de operação da janela.
 */
void Mapa::inicializarAtributos(const QJsonObject &getMapaJson)
{
    QString folder = CCacic::getValueFromRegistry("Lightbase", "Cacic", "mainFolder").toString();
    mainFolder = !folder.isEmpty() && !folder.isNull() ? folder : Identificadores::ENDERECO_PATCH_CACIC;
    logcacic = new LogCacic(LOG_MAPA, mainFolder + "/Logs");

    this->mapaJson = getMapaJson;

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

    geraNovosCampos();
    geraCampoMensagem();
}

/**
 * @brief Mapa::on_okButton_clicked
 *
 * Lida com o clique do usuário para finalizar o Mapa através do botão Ok,
 * salvando as informações se corretamente preenchidas.
 */
void Mapa::on_okButton_clicked()
{//TODO: Incluir as informações preenchidas na listNovosCampos ao jsonMapa.
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

            foreach(NovoCampo customEntry, listNovosCampos) {
                QLineEdit *editLine = dynamic_cast<QLineEdit*>(customEntry.getLineWidget());
qDebug() << editLine->text();
                if(!editLine->text().isNull() && !editLine->text().isEmpty() )
                    patrimonio[customEntry.getTitle()] = QJsonValue::fromVariant(editLine->text());
            }

            jsonMapa["patrimonio"] = patrimonio;
            QMessageBox *box;
            if (salvarInfo(jsonMapa)){
                box = new QMessageBox(QMessageBox::Information, "Sucesso!", "Informações salvas com sucesso.", QMessageBox::Ok);
                logcacic->escrever(LogCacic::InfoLevel, "Dados patrimoniais salvos com sucesso.");
            } else {
                box = new QMessageBox(QMessageBox::Warning, "Erro!", "Falha ao salvar as informações.", QMessageBox::Ok);
                logcacic->escrever(LogCacic::ErrorLevel, "Falha ao salvar as informações patrimoniais.");
            }
            box->setWindowFlags(Qt::Popup);

            if( box->exec() == QMessageBox::Ok )
//                emit finished();
                exit(0);
        }
    }
}

/**
 * @brief Mapa::preencheCampos
 *
 * Preenche os campos com as informações que podem ser obtidas
 * sem intervenção do usuário.
 */
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

/**
 * @brief Mapa::preencheNomeUsuario
 * @return QString retornada da consulta.
 *
 * Realiza uma busca do Nome de Usuário através dos parâmetros
 * configurados para a busca em LDAP.
 */
QString Mapa::preencheNomeUsuario()
{
    QString ldapServer, ldapLogin, ldapPass, ldapBase, ldapFilter, ldapAttr;

    if (!mapaJson.isEmpty()){
        QJsonObject ldapJson;
        if(mapaJson.contains("ldap"))
            ldapJson = mapaJson["ldap"].toObject();

        if(!ldapJson.isEmpty()) {
            ldapBase = ldapJson["base"].toString();
            ldapLogin = ldapJson["login"].toString();
            ldapPass = ldapJson["pass"].toString();
            ldapServer = ldapJson["server"].toString();

            //ARRUMAR DEPOIS PARA DAR SUPORTE A MAIS DE UM FILTRO E ATTR
            ldapFilter = ldapJson["filter"].toArray().first().toString();
            if(ldapFilter == "uid"){
               ldapFilter = "(&(objectClass=*)(uid=";
               ldapFilter.append(ui->lineIdUsuario->text());
               ldapFilter.append("))");
            }
            ldapAttr = ldapJson["attr"].toObject()["name"].toString();
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

    LdapHandler ldapHandler(ldapServer);

    if ( ldapHandler.inicializar() )
        return ldapHandler.busca(ldapLogin,ldapPass,ldapBase,ldapFilter,ldapAttr);

    return QString();
}

/**
 * @brief Mapa::salvarInfo
 * @param jsonMapa QJsonObject&
 * @return bool true se salvou
 *
 * Salva as informações do Mapa no arquivo de coleta.
 */
bool Mapa::salvarInfo(const QJsonObject &jsonMapa)
{
    bool ok = false;
    if (!jsonMapa.isEmpty() && !jsonMapa["patrimonio"].toObject().isEmpty()){

        QJsonObject coleta = CCacic::getJsonFromFile(this->mainFolder + "/coleta.json");
        coleta["patrimonio"] = jsonMapa["patrimonio"];
        ok = CCacic::setJsonToFile(coleta, this->mainFolder + "/coleta.json");

        QVariantMap enviaColeta;
        enviaColeta["enviaColeta"] = ok;
        CCacic::setValueToRegistry("Lightbase", "Cacic", enviaColeta);
    }

    return ok;
}

/**
 * @brief Mapa::validarCampos
 * @param listaValores
 * @return bool se os campos tem valores válidos.
 *
 * Verifica se os campos possuem valores válidos para aquela informação,
 * e retorna uma lista com pares campo e valor do campo.
 */
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
