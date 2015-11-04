#include "installcacic.h"

InstallCacic::InstallCacic(QObject *parent) :
    QObject(parent)
{
    CCacic::salvarVersao("install-cacic");

    cacicMainFolder = CCacic::getValueFromRegistry("Lightbase", "Cacic", "mainFolder").toString();
    if (cacicMainFolder.isEmpty()){
        cacicMainFolder = Identificadores::ENDERECO_PATCH_CACIC;
    }

    QDir dir(cacicMainFolder);
    if (!dir.exists()){
        CCacic::createFolder(cacicMainFolder);
    }

    logcacic = new LogCacic(LOG_INSTALL_CACIC, cacicMainFolder+"/Logs");
}

InstallCacic::~InstallCacic()
{
}

void InstallCacic::run(QStringList argv, int argc) {

    // TODO: Verificar hash no gerente.
    oCacicComm = new CacicComm(LOG_INSTALL_CACIC, this->cacicMainFolder);
    bool ok;
    //valida os parametros repassados
    QMap<QString, QString> param = validaParametros(argv, argc, &ok);

    //se tiver usuario, senha e url e nenhum parâmetro a mais.
    if (ok){
        //inicia a instalação.
        this->install();
    } else if ((param.contains("default")) && (param["default"] == "uninstall")){
        //Se tiver -uninstall como parâmetro, inicia desinstalação.
        logcacic->escrever(LogCacic::InfoLevel, "Desinstalando cacic!");
        this->uninstall();
    } else if ((param.contains("default")) && (param["default"] == "configure")) {
        //Se tiver -configure, inicia configuração (trocar de host, usuário ou senha)
        logcacic->escrever(LogCacic::InfoLevel, QString("Configuração do agente."));
        configurar(param);
    } else if ((param.contains("default")) && (param["default"] == "updateService")) {
        //Se tiver -updateService, verifica a pasta temporária se há algum módulo para update.
        logcacic->escrever(LogCacic::InfoLevel, "Atualizando cacic!");
        updateService();
    } else if ((param.contains("default")) && (param["default"] == "forcaColeta")){
        logcacic->escrever(LogCacic::InfoLevel, "Forçando coleta!");
        forcaColeta();
    } else {
        parametrosIncorretos();
    }
    //logcacic->~LogCacic();
    emit finished();
}

//---------MÉTODOS-----------

void InstallCacic::parametrosIncorretos(){
    std::cout << "\nInstalador do Agente Cacic.\n" << std::endl;
    std::cout << "Parametros incorretos/incompletos. (<obrigatorios> [opcional])\n" << std::endl;
    std::cout << "Instalação: <-host=url_gerente> <-user=usuario> <-pass=senha>" << std::endl;
    std::cout << "Desinstalar: <-uninstall>" << std::endl;
    std::cout << "Configurar: <-configure> [-user=usuario] [-pass=senha] [-host=url_gerente]\n" << std::endl;
    std::cout << "  -host=url_gerente       url_gerente: Caminho para a aplicação do gerente." << std::endl;
    std::cout << "  -user=usuario           usuario: usuário de login no gerente." << std::endl;
    std::cout << "  -pass=senha             senha: senha de login no gerente" << std::endl;
}

/*---------------------------------------------------------------------------------------------------
 * Se tiver -updateService, verifica a pasta temporária se há algum módulo para update.
 * --------------------------------------------------------------------------------------------------
 */
void InstallCacic::updateService()
{
#ifdef Q_OS_WIN
    ServiceController service(QString(CACIC_SERVICE_NAME).toStdWString());
#endif
    bool serviceUpdate = false;
    logcacic->escrever(LogCacic::InfoLevel, "Verificando a existência de módulos na pasta temporária.");
    QDir dir(cacicMainFolder + "/temp");
    dir.setFilter(QDir::Files | QDir::Hidden | QDir::NoSymLinks | QDir::Executable);
    dir.setSorting(QDir::Size | QDir::Reversed);

    QFileInfoList list = dir.entryInfoList();
    for (int i = 0; i<list.size(); i++){
        if(!(list.at(i).fileName().contains("install-cacic"))){
            logcacic->escrever(LogCacic::InfoLevel, "Módulo \"" + list.at(i).filePath() + "\" encontrado para atualização.");
            if((list.at(i).fileName().contains("cacic-service"))){
                serviceUpdate = true;
                logcacic->escrever(LogCacic::InfoLevel, "Parando serviço para atualização.");
#ifdef Q_OS_WIN
                if(service.isRunning()){
                    if (!service.stop()){
                        std::cout << "Não foi possível parar o serviço: " + service.getLastError() << std::endl;
                        logcacic->escrever(LogCacic::ErrorLevel, QString("Não foi possível parar o serviço: " +
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
            if (QFile::exists(cacicMainFolder + "/" + list.at(i).fileName())){
                logcacic->escrever(LogCacic::InfoLevel, "Excluindo versão antiga de "+list.at(i).fileName());
                if (!QFile::remove(cacicMainFolder + "/" + list.at(i).fileName())){
                    logcacic->escrever(LogCacic::ErrorLevel, "Falha ao excluir "+list.at(i).fileName());
                } else {
                    //Garantir a exclusão
                    QThread::sleep(1);
                    //Nova verificação pra ter certeza de que não existe, porque se existir ele não vai copiar.
                    if (!QFile::exists(cacicMainFolder + "/" + list.at(i).fileName())){
                        novoModulo.copy(cacicMainFolder + "/" + list.at(i).fileName());
                        logcacic->escrever(LogCacic::InfoLevel, "Copiando arquivo para " + cacicMainFolder);
                    }
                    //Garantir a cópia
                    QThread::sleep(1);
                    if(QFile::exists(cacicMainFolder + "/" + list.at(i).fileName())){
                        if (!novoModulo.remove())
                            logcacic->escrever(LogCacic::ErrorLevel, "Falha ao excluir "+list.at(i).fileName()+" da pasta temporária.");
                    }
                }
            }
        }
    }
    if (serviceUpdate){
        logcacic->escrever(LogCacic::InfoLevel, "Iniciando o serviço cacic.");
#ifdef Q_OS_WIN
        if(!service.isInstalled()){
            std::cout << "Serviço não instalado, reinstalando..." << std::endl;
            logcacic->escrever(LogCacic::InfoLevel, QString("Serviço não instalado, reinstalando..."));
            if(!service.install(QString(cacicMainFolder+"/cacic-service.exe").toStdWString(),
                                L"CacicDaemon")){
                std::cout << "Não foi possível instalar o serviço: " + service.getLastError() +"" << std::endl;
                logcacic->escrever(LogCacic::ErrorLevel, QString("Não foi possível instalar o serviço: " +
                                                                               QString::fromStdString(service.getLastError())));
            } else {
                std::cout << "Serviço reinstalado com sucesso." << std::endl;
                logcacic->escrever(LogCacic::InfoLevel, QString("Serviço reinstalado com sucesso."));
            }
        } else {
            if (!service.start()){
                std::cout << "Não foi possível iniciar o serviço: " + service.getLastError() << std::endl;
                logcacic->escrever(LogCacic::ErrorLevel, QString("Não foi possível iniciar o serviço: " +
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
void InstallCacic::configurar(const QMap<QString, QString> &param)
{
    if (param.size() > 1){
        QVariantMap reg;
        if (param.contains("host")){
            reg["applicationUrl"] = param["host"];
            logcacic->escrever(LogCacic::InfoLevel, QString("Host alterado para " + param["host"]));
            std::cout << "Url alterada para \"" << param["host"].toStdString() << "\"" << std::endl;
        }
        if (param.contains("user")){
            reg["usuario"] = param["user"];
            logcacic->escrever(LogCacic::InfoLevel, QString("Usuário alterado para " + param["user"]));
            std::cout << "Usuário alterado para \"" << param["user"].toStdString() << "\"" << std::endl;
        }
        if (param.contains("pass")){
            reg["senha"] = param["pass"];
            logcacic->escrever(LogCacic::InfoLevel, QString("Senha alterada."));
            std::cout << "Senha alterada." << std::endl;
        }
        if (reg.size() > 0) {
            CCacic::setValueToRegistry("Lightbase", "Cacic", reg);
            std::cout << "\nRegistro atualizado.\n" << std::endl;
        }
    } else {
        parametrosIncorretos();
    }
}

/*---------------------------------------------------------------------------------------------------
 * Inicia a instalação se possuir os parâmetros -host, -user, -pass
 *---------------------------------------------------------------------------------------------------
 */
void InstallCacic::install()
{
    bool ok;
    //logcacic->escrever(LogCacic::InfoLevel, QString("Inicio de instalacao"));
    std::cout << " - - INSTALL CACIC - -" << std::endl;
    oCacicComm->setUrlGerente(this->argumentos["host"]);
    oCacicComm->setUsuario(this->argumentos["user"]);
    oCacicComm->setPassword(this->argumentos["pass"]);
    std::cout << "Realizando login..." << std::endl;

    //logcacic->escrever(LogCacic::InfoLevel, "Realizando login...");
    QJsonObject jsonLogin = oCacicComm->login(&ok);
    if (ok){
        std::cout << "Login realizado com sucesso..." << std::endl;
        logcacic->escrever(LogCacic::InfoLevel, "Login realizado com sucesso!");
        QJsonObject jsonComm;
        QLogger::QLog_Debug("Install", "Login: " + jsonLogin["reply"].toObject()["chavecript"].toString());
        //conectado, grava a chave na classe;
        //oCacic.setChaveCrypt(jsonLogin["reply"].toObject()["chavecript"].toString());

        jsonComm["computador"] = oCacicComputer.toJsonObject();
        std::cout << "Pegando informações do gerente..." << std::endl;
        logcacic->escrever(LogCacic::InfoLevel, "Pegando informações do gerente...");
        QJsonObject configs = oCacicComm->comm(ROTA_GETCONFIG, &ok, jsonComm, true);
        if (ok){
            QJsonObject configsJson = configs["reply"].toObject()["agentcomputer"].toObject();
            oCacicComm->setUrlGerente(configsJson["applicationUrl"].toString());

            CCacic::createFolder(cacicMainFolder);
            //grava chave em registro;
            QVariantMap registro;
            //registro["key"] = oCacic.getChaveCrypt();
            registro["password"] = oCacicComm->getPassword();
            registro["usuario"] = oCacicComm->getUsuario();
            registro["mainFolder"] = cacicMainFolder;
            registro["applicationUrl"] = oCacicComm->getUrlGerente();
            CCacic::setValueToRegistry("Lightbase", "Cacic", registro);
            std::cout << "Sucesso, salvando configurações em arquivo..." << std::endl;
            logcacic->escrever(LogCacic::InfoLevel, "Sucesso! Salvando configurações em arquivo...");
            CCacic::setJsonToFile(configs["reply"].toObject(), cacicMainFolder + "/getConfig.json");

            //Baixa serviço

            QJsonObject metodoDownload;
            metodoDownload = configsJson["metodoDownload"].toObject();
            oCacicComm->setFtpPass(metodoDownload["senha"].toString());
            oCacicComm->setFtpUser(metodoDownload["usuario"].toString());

            logcacic->escrever(LogCacic::InfoLevel, "Verificando serviço...");
            std::cout << "Verificando serviço..." << std::endl;
#ifdef Q_OS_WIN
            //verifica e start o serviço

            QFile fileService(cacicMainFolder+"/cacic-service.exe");
            ServiceController service(QString(CACIC_SERVICE_NAME).toStdWString());
            ServiceController checkCacic(QString("checkcacic").toStdWString());
            if (fileService.exists()) {
                if (!fileService.open(QFile::ReadOnly)){
                    logcacic->escrever(LogCacic::InfoLevel, "Falha ao abrir o arquivo "+fileService.fileName());
                    logcacic->escrever(LogCacic::ErrorLevel, fileService.errorString());
                }
                //Tenta instalar o serviço
                if (service.isInstalled()){
                    QString hashService;
                    foreach(QJsonValue modulo, configsJson["modulos"].toObject()["cacic"].toArray()){
                        if(modulo.toObject()["name"].toString().contains("cacic-service")){
                            hashService = modulo.toObject()["hash"].toString();
                        }
                    }
                    if (!CCacic::Md5IsEqual(fileService.readAll(), hashService)){
                        if (service.isRunning()) {
                            logcacic->escrever(LogCacic::InfoLevel, "Servico atual desatualizado. Preparando atualização.");
                            service.stop();
                            checkCacic.stop();
                        }
                    } else {
                        logcacic->escrever(LogCacic::InfoLevel, "Servico ja instalado e atualizado.");
                        if (!service.isRunning()) service.start();
                        emit finished();
                        return;
                    }
                }
            }
            logcacic->escrever(LogCacic::InfoLevel, "Realizando download do servico.");
            QThread::sleep(2);
            oCacicComm->fileDownload(metodoDownload["tipo"].toString(),
                    metodoDownload["url"].toString(),
                    metodoDownload["path"].toString() +
                        (metodoDownload["path"].toString().endsWith("/") ? "" : "/") +
                        "cacic-service.exe",
                    cacicMainFolder);
            QThread::sleep(1);
            if (fileService.exists()){
                if (!service.isInstalled()){
                    if (!service.install(QString(cacicMainFolder+"/cacic-service.exe").toStdWString(),
                                         QString(CACIC_SERVICE_NAME).toStdWString())){
                        logcacic->escrever(LogCacic::ErrorLevel, "Falha ao reinstalar o serviço: " +
                                                                                QString::fromStdString(service.getLastError()));
                        uninstall();
                    }
                }
                if (service.start()){
                    std::cout << "Instalação realizada com sucesso." << std::endl;;
                    logcacic->escrever(LogCacic::InfoLevel, QString("Instalação realizada com sucesso."));
                } else {
                    logcacic->escrever(LogCacic::InfoLevel, "Falha ao iniciar o serviço. Reinicie o computador!");
                    logcacic->escrever(LogCacic::ErrorLevel, "Falha ao iniciar o serviço: " +
                                                                            QString::fromStdString(service.getLastError()));
                }
            } else {
                logcacic->escrever(LogCacic::InfoLevel, "Falha ao baixar servico.");
                logcacic->escrever(LogCacic::ErrorLevel, "Falha ao baixar servico.");
            }
    #else

            oCacicComm->fileDownload(metodoDownload["tipo"].toString(),
                    metodoDownload["url"].toString(),
                    metodoDownload["path"].toString() +
                    (metodoDownload["path"].toString().endsWith("/") ? "" : "/") + "cacic-service",
                    cacicMainFolder);

            QFile fileService(cacicMainFolder+"/cacic-service");
            if ((!fileService.exists() || !fileService.size() > 0)) {
                std::cout << "Falha ao baixar arquivo."  << std::endl;
                logcacic->escrever(LogCacic::ErrorLevel, "Falha ao baixar o serviço...");
                this->uninstall();
                fileService.close();
            } else {
                std::cout << "Iniciando serviço..." << std::endl;
                ConsoleObject console;
                std::cout << console("/etc/init.d/cacic3 start").toStdString();
            }

#endif          

        } else {
            std::cout << "Falha ao pegar configurações: " << configs["error"].toString().toStdString() << std::endl;
            logcacic->escrever(LogCacic::ErrorLevel, QString("Falha ao pegar configurações: ") + configs["error"].toString());
        }

    } else {
        std::cout << "Nao foi possivel realizar o login." << std::endl;
        std::cout << "  Código: " << jsonLogin["codestatus"].toString().toStdString() << std::endl;
        std::cout << "  " << jsonLogin["error"].toString().toStdString() << std::endl;
        logcacic->escrever(LogCacic::ErrorLevel, QString("Falha no login: ") + jsonLogin["error"].toString());
    }
}

void InstallCacic::forcaColeta()
{
    QLocalSocket socket;
    socket.setServerName(CACIC_SERVICE_NAME);
    socket.connectToServer();
    if (socket.isOpen()){
        socket.write("Coletar");
        socket.flush();
        if (!socket.waitForBytesWritten(3000))
            logcacic->escrever(LogCacic::InfoLevel, "Falha ao enviar sinal de coleta.");
        socket.close();
        logcacic->escrever(LogCacic::InfoLevel, "Sinal para forçar coleta enviado.");
    } else {
        logcacic->escrever(LogCacic::ErrorLevel, "Não foi possível conectar ao server.");
    }
}

/*---------------------------------------------------------------------------------------------------
 * Retorna true se possuir -host, -user, -pass e não possuir nenhum outro argumento.
 * Os demais argumentos, que não possuírem atribuição (se for só -argumento e não -argumento=algumacoisa)
 * serão setados como ["default"] = atributo.
 *---------------------------------------------------------------------------------------------------
 */
QMap<QString, QString> InstallCacic::validaParametros(QStringList argv, int argc, bool *ok)
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

void InstallCacic::uninstall()
{
#ifdef Q_OS_WIN
    ServiceController service(QString(CACIC_SERVICE_NAME).toStdWString());
    if (service.isInstalled()){
        logcacic->escrever(LogCacic::InfoLevel, QString("Desinstalando o serviço..."));
        if (!service.uninstall()){
            std::cout << "Não foi possível parar o serviço: " + service.getLastError() << std::endl;
            logcacic->escrever(LogCacic::ErrorLevel, QString("Não foi possível parar o serviço: " +
                                                                           QString::fromStdString(service.getLastError())));
        }
    }

#elif defined(Q_OS_LINUX)
    ConsoleObject console;
    QStringList outputColumns;
    console("killall -eq cacic-service gercols");
    std::cout << "Parando serviço...";
    std::cout << console("/etc/init.d/cacic3 stop").toStdString();

    outputColumns = console("ps aux | grep cacic-service").split("\n");
    outputColumns.removeLast();

    foreach(QString processString, outputColumns) {

        if(processString.contains("grep"))
            continue;

        QStringList columns = processString.split(" ");
        int i = 0;
        foreach(QString column, columns){
            if( !column.isEmpty() ) {
                i++;
                if( i == 2 ) {
                    qDebug() << column;
                    console("kill -9 " + column);
                    logcacic->escrever(LogCacic::InfoLevel, QString("Cacic-service interrompido."));
                }
            }
        }
    }
#endif
    CCacic::removeRegistry("Lightbase", "Cacic");

    // Exclui tudo no diretorio, menos o install-cacic
    if (!cacicMainFolder.isEmpty()) {

        QDir dir(cacicMainFolder);
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
                    CCacic::deleteFolder(list.at(i).absoluteFilePath());
                else
                    CCacic::deleteFile(list.at(i).absoluteFilePath());
            }
        }
    }

    std::cout << "\nCacic desinstalado com sucesso." << std::endl;
}

QMap<QString, QString> InstallCacic::getArgumentos()
{
    return argumentos;
}

void InstallCacic::setArgumentos(QMap<QString, QString> value)
{
    this->argumentos = value;
}
