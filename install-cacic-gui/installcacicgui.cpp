#include "installcacicgui.h"
#include "ui_installcacicgui.h"

InstallCacicGui::InstallCacicGui(QWidget *parent) : QMainWindow(parent), ui(new Ui::InstallCacicGui)
{
    ui->setupUi(this);
    ui->centralWidget->setWindowFlags(Qt::WindowMaximizeButtonHint);
    oCacic.setCacicMainFolder(oCacic.getValueFromRegistry("Lightbase", "Cacic", "mainFolder").toString());
    if (oCacic.getCacicMainFolder().isEmpty()){
        this->applicationDirPath = Identificadores::ENDERECO_PATCH_CACIC;
        oCacic.setCacicMainFolder(applicationDirPath);
    } else {
        this->applicationDirPath = oCacic.getCacicMainFolder();
    }
    QDir dir(oCacic.getCacicMainFolder());
    if (!dir.exists()){
        oCacic.createFolder(oCacic.getCacicMainFolder());
    }
    logManager = QLogger::QLoggerManager::getInstance();
    logManager->addDestination(oCacic.getCacicMainFolder() + "/Logs/cacic.log", Identificadores::LOG_INSTALL_CACIC ,QLogger::InfoLevel);
    logManager->addDestination(oCacic.getCacicMainFolder() + "/Logs/cacic.log", Identificadores::LOG_INSTALL_CACIC ,QLogger::ErrorLevel);
    trayMenu = new QMenu(this);
    tray = new QSystemTrayIcon(this);
    tray->setIcon(QIcon(":/cacic-logo.ico"));
    trayIcon(true, QString("Sobre o cacic"), true, QString("Cacic"), QString("Cacic iniciado com sucesso."));tray->show();
}

InstallCacicGui::~InstallCacicGui()
{
    delete ui;
}

void InstallCacicGui::on_pbInstalar_clicked()
{
    QStringList parametros;
    if(getModoDeExecucao() == Identificadores::DESINSTALAR){
        parametros.clear();
        parametros.append(QString("-uninstall"));
        run(parametros, parametros.size());
        return;
    }
    if(verificarDadosInseridos()){
        parametros.append(QString("-host=" + ui->leHost->text()));
        parametros.append(QString("-user=" + ui->leUsuario->text()));
        parametros.append(QString("-pass=" + ui->leSenha->text()));
        if(getModoDeExecucao() == Identificadores::INSTALAR){
            run(parametros, parametros.size());
            return;
        }
        if(getModoDeExecucao() ==  Identificadores::MODIFICAR){
            parametros.prepend(QString("-configure"));
            run(parametros, parametros.size());
            return;
        }
    }else{
        mensagemDeProgresso("Preencha os campos corretamente.", true, true);
        return;
    }
}

bool InstallCacicGui::verificarDadosInseridos(){
    if(!ui->leHost->text().isEmpty() && !ui->leHost->text().isNull()){
        if(!ui->leUsuario->text().isEmpty() && !ui->leUsuario->text().isNull()){
            if(!ui->leSenha->text().isEmpty() && !ui->leSenha->text().isNull()){
                return true;
            }
        }
    }
    return false;
}

void InstallCacicGui::resolverModoDeExecucao(){
    if(getModoDeExecucao() == Identificadores::INSTALAR){
        ui->cbHost->setDisabled(true);
        ui->cbHost->setVisible(false);
        ui->cbUsu->setDisabled(true);
        ui->cbUsu->setVisible(false);
        ui->cbPass->setDisabled(true);
        ui->cbPass->setVisible(false);
        ui->leHost->setPlaceholderText(QString("Endereço do gerente"));
        ui->leUsuario->setPlaceholderText(QString("usuário"));
        ui->leSenha->setPlaceholderText(QString("senha"));
    }
    if(getModoDeExecucao() == Identificadores::MODIFICAR){
        ui->cbHost->setDisabled(false);
        ui->cbHost->setVisible(true);
        ui->cbUsu->setDisabled(false);
        ui->cbUsu->setVisible(true);
        ui->cbPass->setDisabled(false);
        ui->cbPass->setVisible(true);
        ui->leHost->setEnabled(true);
        ui->leHost->setText(oCacic.getValueFromRegistry("Lightbase", "Cacic", "applicationUrl").toString());
        ui->leHost->setReadOnly(true);
        ui->leUsuario->setEnabled(true);
        ui->leUsuario->setText(oCacic.getValueFromRegistry("Lightbase", "Cacic", "usuario").toString());
        ui->leUsuario->setReadOnly(true);
        ui->leSenha->setEnabled(true);
        ui->leSenha->setText(oCacic.getValueFromRegistry("Lightbase", "Cacic", "password").toString());
        ui->leSenha->setReadOnly(true);
        ui->pbInstalar->setText("Modificar configurações");
        ui->pbCancelar->setText("Cancelar");
        ui->pbCancelar->setEnabled(true);
        ui->pbCancelar->setVisible(true);
    }
    if(getModoDeExecucao() == Identificadores::DESINSTALAR){
        ui->leHost->setEnabled(false);
        ui->leHost->setReadOnly(true);
        ui->leHost->setText(oCacic.getValueFromRegistry("Lightbase", "Cacic", "applicationUrl").toString());
        ui->leUsuario->setEnabled(false);
        ui->leUsuario->setReadOnly(true);
        ui->leUsuario->setText(oCacic.getValueFromRegistry("Lightbase", "Cacic", "usuario").toString());
        ui->leSenha->setEnabled(false);
        ui->leSenha->setReadOnly(true);
        ui->leSenha->setText(oCacic.getValueFromRegistry("Lightbase", "Cacic", "password").toString());
        ui->cbHost->setVisible(true);
        ui->cbHost->setEnabled(false);
        ui->cbUsu->setVisible(true);
        ui->cbUsu->setEnabled(false);
        ui->cbPass->setVisible(true);
        ui->cbPass->setEnabled(false);
        ui->pbInstalar->setText("Desinstalar o Cacic");
        ui->pbCancelar->setText("Cancelar");
        ui->pbCancelar->setEnabled(true);
        ui->pbCancelar->setVisible(true);
    }
    ui->pteResult->setReadOnly(true);
    ui->pteResult->setFont(QFont("Arial", 10));
}

void InstallCacicGui::run(QStringList argv, int argc) {
    // TODO: Verificar hash no gerente.
    oCacicComm = new CacicComm();
    bool ok;
    //valida os parametros repassados
    QMap<QString, QString> param = validaParametros(argv, argc, &ok);

    //se tiver usuario, senha e url e nenhum parâmetro a mais.
    if (ok){
        //inicia a instalação.
        if(!oCacic.verificarRoot()){
            mensagemDeProgresso("O Cacic deve ser instalado com permissões de administrador.\n\n");
            if(!isGui()){
                emit finished();
            }
        }else{
            this->install();
        }
    } else if ((param.contains("default")) && (param["default"] == "uninstall")){
        //Se tiver -uninstall como parâmetro, inicia desinstalação.
        QLogger::QLog_Info(Identificadores::LOG_INSTALL_CACIC, "Desinstalando cacic!");
        this->uninstall();
    } else if ((param.contains("default")) && (param["default"] == "configure")) {
        //Se tiver -configure, inicia configuração (trocar de host, usuário ou senha)
        QLogger::QLog_Info(Identificadores::LOG_INSTALL_CACIC, QString("Configuração do agente."));
        configurar(param);
    } else if ((param.contains("default")) && (param["default"] == "updateService")) {
        //Se tiver -updateService, verifica a pasta temporária se há algum módulo para update.
        QLogger::QLog_Info(Identificadores::LOG_INSTALL_CACIC, "Atualizando cacic!");
        updateService();
    } else {
        if(!isGui()) {
            parametrosIncorretos();
        }else{
            mensagemDeProgresso("Parametros de configuração inválidos.",true, true);
        }
    }
    logManager->closeLogger();
    logManager->wait();
    if(!isGui()){
        emit finished();
    }
}

//---------MÉTODOS-----------

void InstallCacicGui::parametrosIncorretos(){
    std::cout << "\nInstalador do Agente Cacic.\n\n"
              << "Parametros incorretos/incompletos. (<obrigatorios> [opcional])\n\n"
              << "Instalação: <-host=url_gerente> <-user=usuario> <-pass=senha>\n"
              << "Desinstalar: <-uninstall>\n"
              << "Configurar: <-configure> [-user=usuario] [-pass=senha] [-host=url_gerente]\n\n"
              << "  -host=url_gerente       url_gerente: Caminho para a aplicação do gerente.\n"
              << "  -user=usuario           usuario: usuário de login no gerente.\n"
              << "  -pass=senha             senha: senha de login no gerente\n\n";
}

/*---------------------------------------------------------------------------------------------------
 * Se tiver -updateService, verifica a pasta temporária se há algum módulo para update.
 * --------------------------------------------------------------------------------------------------
 */
void InstallCacicGui::updateService()
{
#ifdef Q_OS_WIN
    bool ok;
    ServiceController service(Identificadores::CACIC_SERVICE_NAME.toStdWString());
#endif
    bool serviceUpdate = false;
    QLogger::QLog_Info(Identificadores::LOG_INSTALL_CACIC, "Verificando a existência de módulos na pasta temporária.");
    QDir dir(oCacic.getCacicMainFolder() + "/temp");
    dir.setFilter(QDir::Files | QDir::Hidden | QDir::NoSymLinks | QDir::Executable);
    dir.setSorting(QDir::Size | QDir::Reversed);

    QFileInfoList list = dir.entryInfoList();
    for (int i = 0; i<list.size(); i++){
        if(!(list.at(i).fileName().contains("install-cacic"))){
            QLogger::QLog_Info(Identificadores::LOG_INSTALL_CACIC, "Módulo \"" + list.at(i).filePath() + "\" encontrado para atualização.");
            if((list.at(i).fileName().contains("cacic-service"))){
                serviceUpdate = true;
                QLogger::QLog_Info(Identificadores::LOG_INSTALL_CACIC, "Parando serviço para atualização.");
#ifdef Q_OS_WIN
                ok = false;
                if(service.isRunning()){
                    if (!service.stop()){
                        std::cout << "Não foi possível parar o serviço: " + service.getLastError() +"\n";
                        QLogger::QLog_Info(Identificadores::LOG_INSTALL_CACIC, QString("Não foi possível parar o serviço: " +
                                                                                       QString::fromStdString(service.getLastError())));
                    }
                }
#else
                ConsoleObject console;
                console("/etc/init.d/cacic3 stop");
#endif
            }
            //Ter certeza de que o serviço parou
            QThread::sleep(3);
            QFile novoModulo(list.at(i).filePath());
            if (QFile::exists(applicationDirPath + "/" + list.at(i).fileName())){
                QLogger::QLog_Info(Identificadores::LOG_INSTALL_CACIC, "Excluindo versão antiga de "+list.at(i).fileName());
                if (!QFile::remove(applicationDirPath + "/" + list.at(i).fileName())){
                    QLogger::QLog_Info(Identificadores::LOG_INSTALL_CACIC, "Falha ao excluir "+list.at(i).fileName());
                } else {
                    //Garantir a exclusão
                    QThread::sleep(1);
                    //Nova verificação pra ter certeza de que não existe, porque se existir ele não vai copiar.
                    if (!QFile::exists(applicationDirPath + "/" + list.at(i).fileName())){
                        novoModulo.copy(applicationDirPath + "/" + list.at(i).fileName());
                        QLogger::QLog_Info(Identificadores::LOG_INSTALL_CACIC, "Copiando arquivo para " + applicationDirPath);
                    }
                    //Garantir a cópia
                    QThread::sleep(1);
                    if(QFile::exists(applicationDirPath + "/" + list.at(i).fileName())){
                        if (!novoModulo.remove())
                            QLogger::QLog_Info(Identificadores::LOG_INSTALL_CACIC, "Falha ao excluir "+list.at(i).fileName()+" da pasta temporária.");
                    }
                }
            }
        }
    }
    if (serviceUpdate){
        QLogger::QLog_Info(Identificadores::LOG_INSTALL_CACIC, "Iniciando o serviço cacic.");
#ifdef Q_OS_WIN
        if(!service.isInstalled()){
            mensagemDeProgresso("Serviço não instalado, reinstalando...");
            QLogger::QLog_Info(Identificadores::LOG_INSTALL_CACIC, QString("Serviço não instalado, reinstalando..."));
            if(!service.install(QString(oCacic.getCacicMainFolder()+"/cacic-service.exe").toStdWString(),
                                L"Cacic Daemon")){
                mensagemDeProgresso("Não foi possível instalar o serviço: " + QString::fromStdString(service.getLastError()));
                QLogger::QLog_Info(Identificadores::LOG_INSTALL_CACIC, QString("Não foi possível instalar o serviço: " +
                                                                               QString::fromStdString(service.getLastError())));
            } else {
                mensagemDeProgresso("Serviço reinstalado com sucesso.");
                QLogger::QLog_Info(Identificadores::LOG_INSTALL_CACIC, QString("Serviço reinstalado com sucesso."));
            }
        } else {
            if (!service.start()){
                mensagemDeProgresso("Não foi possível iniciar o serviço: " + QString::fromStdString(service.getLastError()));
                QLogger::QLog_Info(Identificadores::LOG_INSTALL_CACIC, QString("Não foi possível iniciar o serviço: " +
                                                                               QString::fromStdString(service.getLastError())));
            }
        }
#else
        ConsoleObject console;
        console("/etc/init.d/cacic3 start");
#endif
    }
}
/*---------------------------------------------------------------------------------------------------
 * Se possuir -configure e algum parâmetro a mais (-host, -user, -pass) realiza a troca das informações
 *---------------------------------------------------------------------------------------------------
 */
void InstallCacicGui::configurar(const QMap<QString, QString> &param)
{
    if (param.size() > 1){
        QVariantMap reg;
        if (param.contains("host")){
            reg["applicationUrl"] = param["host"];
            QLogger::QLog_Info(Identificadores::LOG_INSTALL_CACIC, QString("Host alterado para " + param["host"]));
            mensagemDeProgresso(QString("Url alterada para \"" + param["host"] + "\""), true, true);
        }
        if (param.contains("user")){
            reg["usuario"] = param["user"];
            QLogger::QLog_Info(Identificadores::LOG_INSTALL_CACIC, QString("Usuário alterado para " + param["user"]));
            mensagemDeProgresso("Usuário alterado para \"" + param["user"] + "\"");
        }
        if (param.contains("pass")){
            reg["senha"] = param["pass"];
            QLogger::QLog_Info(Identificadores::LOG_INSTALL_CACIC, QString("Senha alterada."));
            mensagemDeProgresso("Senha alterada.\n");
        }
        if (reg.size() > 0) {
            oCacic.setValueToRegistry("Lightbase", "Cacic", reg);
            mensagemDeProgresso("Registro atualizado.\n\n");
        }
    } else {
        parametrosIncorretos();
    }
}

/*---------------------------------------------------------------------------------------------------
 * Inicia a instalação se possuir os parâmetros -host, -user, -pass
 *---------------------------------------------------------------------------------------------------
 */
void InstallCacicGui::install()
{
    bool ok;
    QLogger::QLog_Info(Identificadores::LOG_INSTALL_CACIC, QString("Inicio de instalacao"));
    mensagemDeProgresso(" - - INSTALL CACIC - -", true, true);
    oCacicComm->setUrlGerente(this->argumentos["host"]);
    oCacicComm->setUsuario(this->argumentos["user"]);
    oCacicComm->setPassword(this->argumentos["pass"]);
    mensagemDeProgresso("Realizando login...");
    QLogger::QLog_Info(Identificadores::LOG_INSTALL_CACIC, "Realizando login...");
    QJsonObject jsonLogin = oCacicComm->login(&ok);
    if (ok){
        mensagemDeProgresso("Login realizado com sucesso...");
        QLogger::QLog_Info(Identificadores::LOG_INSTALL_CACIC, "Login realizado com sucesso!");
        QJsonObject jsonComm;
        QLogger::QLog_Debug("Install", "Login: " + jsonLogin["reply"].toObject()["chavecript"].toString());
        //conectado, grava a chave na classe;
        oCacic.setChaveCrypt(jsonLogin["reply"].toObject()["chavecript"].toString());

        jsonComm["computador"] = oCacicComputer.toJsonObject();
        mensagemDeProgresso("Pegando informações do gerente...");
        QLogger::QLog_Info(Identificadores::LOG_INSTALL_CACIC, "Pegando informações do gerente...");
        QJsonObject configs = oCacicComm->comm(Identificadores::ROTA_GETCONFIG, &ok, jsonComm, true);
        if (ok){
            QJsonObject configsJson = configs["reply"].toObject()["agentcomputer"].toObject();
            oCacicComm->setUrlGerente(configsJson["applicationUrl"].toString());
            oCacic.setCacicMainFolder(Identificadores::ENDERECO_PATCH_CACIC);
            oCacic.createFolder(oCacic.getCacicMainFolder());
            //grava chave em registro;
            QVariantMap registro;
            registro["key"] = oCacic.getChaveCrypt();
            registro["password"] = oCacicComm->getPassword();
            registro["usuario"] = oCacicComm->getUsuario();
            registro["mainFolder"] = oCacic.getCacicMainFolder();
            registro["applicationUrl"] = oCacicComm->getUrlGerente();
            oCacic.setValueToRegistry("Lightbase", "Cacic", registro);
            mensagemDeProgresso("Sucesso, salvando configurações em arquivo...");
            QLogger::QLog_Info(Identificadores::LOG_INSTALL_CACIC, "Sucesso! Salvando configurações em arquivo...");
            oCacic.setJsonToFile(configs["reply"].toObject(), oCacic.getCacicMainFolder() + "/getConfig.json");

            //Baixa serviço

            QJsonObject metodoDownload;
            metodoDownload = configsJson["metodoDownload"].toObject();
            oCacicComm->setFtpPass(metodoDownload["senha"].toString());
            oCacicComm->setFtpUser(metodoDownload["usuario"].toString());
            mensagemDeProgresso("Realizando download do serviço...");
            QLogger::QLog_Info(Identificadores::LOG_INSTALL_CACIC, "Realizando download do serviço...");
            mensagemDeProgresso("Baixando serviço...");
#ifdef Q_OS_WIN
            oCacicComm->fileDownload(metodoDownload["tipo"].toString(),
                    metodoDownload["url"].toString(),
                    metodoDownload["path"].toString() +
                    (!metodoDownload["path"].toString().endsWith("/") ? "/" : "") +
                    "cacic-service.exe",
                    oCacic.getCacicMainFolder());

            //verifica e start o serviço

            QFile fileService(oCacic.getCacicMainFolder()+"/cacic-service.exe");
            if ((!fileService.exists() || !fileService.size() > 0)) {
                mensagemDeProgresso("Falha ao baixar arquivo.");
                QLogger::QLog_Info(Identificadores::LOG_INSTALL_CACIC, "Falha ao baixar o serviço...");
                fileService.close();
                this->uninstall();
            } else {
                ServiceController service(Identificadores::CACIC_SERVICE_NAME.toStdWString());
                //Tenta instalar o serviço
                if (service.isInstalled()){
                    mensagemDeProgresso("Reinstalando serviço.");
                    QLogger::QLog_Info(Identificadores::LOG_INSTALL_CACIC, "Reinstalando serviço.");
                    if (!service.uninstall()){
                        QLogger::QLog_Info(Identificadores::LOG_INSTALL_CACIC, "Falha ao desinstalar o serviço: " +
                                           QString::fromStdString(service.getLastError()));
                        uninstall();
                    }
                } else {
                    mensagemDeProgresso("Instalando serviço.");
                    QLogger::QLog_Info(Identificadores::LOG_INSTALL_CACIC, QString("Instalando serviço."));
                }
                //tenta a instalação novamente.

                if (!service.install(QString(oCacic.getCacicMainFolder()+"/cacic-service.exe").toStdWString(),
                                     QString("Cacic Daemon").toStdWString())){
                    QLogger::QLog_Info(Identificadores::LOG_INSTALL_CACIC, "Falha ao reinstalar o serviço: " +
                                       QString::fromStdString(service.getLastError()));
                    uninstall();
                }
                if (service.start()){
                    mensagemDeProgresso("Instalação realizada com sucesso.");
                    QLogger::QLog_Info(Identificadores::LOG_INSTALL_CACIC, QString("Instalação realizada com sucesso."));
                    if(isGui()){
                        if (QMessageBox::Ok == QMessageBox(
                                    QMessageBox::Information,
                                    "Instalação do Cacic",
                                    "Cacic instalado com sucesso.",
                                    QMessageBox::Ok).exec()){
                            emit finished();
                        }
                    }else{
                        emit finished();
                    }
                } else {
                    QLogger::QLog_Info(Identificadores::LOG_INSTALL_CACIC, "Falha ao iniciar o serviço: " +
                                       QString::fromStdString(service.getLastError()));
                    uninstall();
                }
            }
#else

            oCacicComm->fileDownload(metodoDownload["tipo"].toString(),
                    metodoDownload["url"].toString(),
                    metodoDownload["path"].toString() +
                    (metodoDownload["path"].toString().endsWith("/") ? "" : "/") + "cacic-service",
                    oCacic.getCacicMainFolder());

            QFile fileService(oCacic.getCacicMainFolder()+"/cacic-service");
            if ((!fileService.exists() || !fileService.size() > 0)) {
                mensagemDeProgresso("Falha ao baixar arquivo.");
                QLogger::QLog_Info(Identificadores::LOG_INSTALL_CACIC, "Falha ao baixar o serviço...");
                this->uninstall();
                fileService.close();
            } else {
                mensagemDeProgresso("Iniciando serviço...");
                ConsoleObject console;
                console("/etc/init.d/cacic3 start");
                mensagemDeProgresso("Instalado com sucesso.\n\n");
                if(isGui()){
                    if (QMessageBox::Ok == QMessageBox(
                                QMessageBox::Information,
                                "Instalação do Cacic",
                                "Cacic instalado com sucesso.",
                                QMessageBox::Ok).exec()){
                        emit finished();
                    }
                }else{
                    emit finished();
                }
            }
#endif
        } else {
            mensagemDeProgresso("Falha ao pegar configurações: " + configs["error"].toString());
            QLogger::QLog_Info(Identificadores::LOG_INSTALL_CACIC, QString("Falha ao pegar configurações: ") + configs["error"].toString());
        }

    } else {
        mensagemDeProgresso("Nao foi possivel realizar o login.\nCódigo: " + jsonLogin["codestatus"].toString() + "\n" + jsonLogin["error"].toString());
        QLogger::QLog_Info(Identificadores::LOG_INSTALL_CACIC, QString("Falha no login: ") + jsonLogin["error"].toString());
    }
}

void InstallCacicGui::mensagemDeProgresso(QString msg, bool limparMsgAnterior, bool primeiraLinha)
{
    if(limparMsgAnterior){
        ui->pteResult->clear();
    }
    if(isGui()){
        ui->pteResult->moveCursor(QTextCursor::End);
        if(primeiraLinha){
            ui->pteResult->insertPlainText(msg);
        }else{
            ui->pteResult->insertPlainText("\n" + msg);
        }
    }else{
        std::cout << "\n" + msg.toStdString();
    }
}

/*---------------------------------------------------------------------------------------------------
 * Retorna true se possuir -host, -user, -pass e não possuir nenhum outro argumento.
 * Os demais argumentos, que não possuírem atribuição (se for só -argumento e não -argumento=algumacoisa)
 * serão setados como ["default"] = atributo.
 *---------------------------------------------------------------------------------------------------
 */
QMap<QString, QString> InstallCacicGui::validaParametros(QStringList argv, int argc, bool *ok)
{
    QMap<QString, QString> map;
    for (int i = 0; i<argc; i++){
        QString aux = argv[i];
        QStringList auxList = aux.split("=");
        if ((auxList.at(0).at(0) == '-') && (auxList.size() > 1))
            map[auxList.at(0).mid(1)] = auxList.at(1);
        else if (aux.at(0)== '-')
            map["default"] = aux.mid(1);
    }
    *ok = (bool) map.contains("host") && map.contains("user") && map.contains("pass") && !(map.contains("default"));
    if (*ok){
        this->argumentos = map;
    }
    return map;
}

void InstallCacicGui::uninstall()
{
    ConsoleObject console;
    mensagemDeProgresso("Desinstalando, aguarde ...", true, true);
#ifdef Q_OS_WIN
    ServiceController service(Identificadores::CACIC_SERVICE_NAME.toStdWString());
    QLogger::QLog_Info(Identificadores::LOG_INSTALL_CACIC, QString("Desinstalando o serviço..."));
    console("TASKKILL /F /IM cacic-service.exe");
    if (!service.uninstall()){
        mensagemDeProgresso("Não foi possível parar o serviço: " + QString::fromStdString(service.getLastError()));
        QLogger::QLog_Info(Identificadores::LOG_INSTALL_CACIC, QString("Não foi possível parar o serviço: " +
                                                                       QString::fromStdString(service.getLastError())));
        return;
    }

#elif defined(Q_OS_LINUX)
    //QStringList outputColumns;
    mensagemDeProgresso("Parando serviço...");
    console("killall -eq cacic-service gercols");
    mensagemDeProgresso(QString(console("/etc/init.d/cacic3 stop")));

    //outputColumns = console("ps aux | grep cacic-service").split("\n");
    // outputColumns.removeLast();

    //    foreach(QString processString, outputColumns) {

    //        if(processString.contains("grep"))
    //            continue;

    //        QStringList columns = processString.split(" ");
    //        int i = 0;
    //        foreach(QString column, columns){
    //            if( !column.isEmpty() ) {
    //                i++;
    //                if( i == 2 ) {
    //                    mensagemDeProgresso(column);
    //                    console("kill -9 " + column);
    //                    QLogger::QLog_Info(Identificadores::LOG_INSTALL_CACIC, QString("Cacic-service interrompido."));
    //                }
    //            }
    //        }
    //    }
#endif
    oCacic.removeRegistry("Lightbase", "Cacic");

    // Exclui tudo no diretorio, menos o install-cacic
    if (!oCacic.getCacicMainFolder().isEmpty()) {

        QDir dir(oCacic.getCacicMainFolder());
        dir.setFilter(QDir::AllEntries | QDir::Hidden );
        dir.setSorting(QDir::Size | QDir::Reversed);

        QFileInfoList list = dir.entryInfoList();

        for (int i = 0; i<list.size(); i++) {

            if( list.at(i).fileName() != "." &&
                    list.at(i).fileName() != ".." &&
                    list.at(i).fileName() != "bin" &&
                    !list.at(i).fileName().contains("install-cacic") &&
                    !list.at(i).fileName().contains("Logs") &&
                    !list.at(i).fileName().contains("cacic.log") ) {

                if ( list.at(i).isDir())
                    oCacic.deleteFolder(list.at(i).absoluteFilePath());
                else
                    oCacic.deleteFile(list.at(i).absoluteFilePath());
            }
        }
    }

    mensagemDeProgresso("Cacic desinstalado com sucesso.\n");
    if(isGui()){
        if (QMessageBox::Ok == QMessageBox(
                    QMessageBox::Information,
                    "Desistalação do Cacic",
                    "Cacic Desinstalado com sucesso.",
                    QMessageBox::Ok).exec()){
            emit finished();
        }
    }else{
        emit finished();
    }
}

int InstallCacicGui::getModoDeExecucao() const
{
    return modoDeExecucao;
}

void InstallCacicGui::setModoDeExecucao(int value)
{
    modoDeExecucao = value;
    resolverModoDeExecucao();
}

bool InstallCacicGui::isGui() const
{
    return gui;
}

void InstallCacicGui::setGui(bool value)
{
    gui = value;
}

QMap<QString, QString> InstallCacicGui::getArgumentos()
{
    return argumentos;
}

void InstallCacicGui::setArgumentos(QMap<QString, QString> value)
{
    this->argumentos = value;
}

void InstallCacicGui::on_cbHost_stateChanged(int arg1)
{
    resolverAcoesAoSelecionarCheckBox(arg1, ui->leHost, QString("applicationUrl"));
}

void InstallCacicGui::on_cbUsu_stateChanged(int arg1)
{
    resolverAcoesAoSelecionarCheckBox(arg1, ui->leUsuario, QString("usuario"));
}

void InstallCacicGui::on_cbPass_stateChanged(int arg1)
{
    resolverAcoesAoSelecionarCheckBox(arg1, ui->leSenha, QString("password"));
}

void InstallCacicGui::resolverAcoesAoSelecionarCheckBox(int arg1, QLineEdit *le, QString registro){
    if(arg1 >= 1){
        le->setReadOnly(false);
        le->clear();
        le->setFocus();
    }else{
        le->setReadOnly(true);
        if(registro == "applicationUrl"){
            le->setText(oCacic.getValueFromRegistry("Lightbase", "Cacic", "applicationUrl").toString());
        }
        if(registro == "usuario"){
            le->setText(oCacic.getValueFromRegistry("Lightbase", "Cacic", "usuario").toString());
        }
        if(registro == "password"){
            le->setText(oCacic.getValueFromRegistry("Lightbase", "Cacic", "password").toString());
        }
    }
}

void InstallCacicGui::on_pbCancelar_clicked()
{
    if(getModoDeExecucao() == Identificadores::INSTALAR){
        emit finished();
    }else{
        this->close();
    }
}

void InstallCacicGui::trayIcon(bool addMenu, QString menu, bool sendMsg, QString titulo, QString msg){
    if(addMenu){
        if(!menu.isEmpty() && !menu.isNull()){
            trayMenu->addAction(menu);
        }else{
            return;
        }
        tray->setContextMenu(trayMenu);
    }
    tray->show();
    if(sendMsg){
        if(!titulo.isEmpty() && !titulo.isNull() && !msg.isNull() && !msg.isEmpty()){
            tray->showMessage(titulo, msg);
        }
    }
}
