#include "installcacic.h"

InstallCacic::InstallCacic(QObject *parent) :
    QObject(parent)
{
    oCacic.setCacicMainFolder(oCacic.getValueFromRegistry("Lightbase", "Cacic", "mainFolder").toString());

    if (oCacic.getCacicMainFolder().isEmpty()){
#ifdef Q_OS_WIN
        this->applicationDirPath = "c:/cacic";
#else
        this->applicationDirPath = "/usr/share/cacic";
#endif
        oCacic.setCacicMainFolder(applicationDirPath);
    } else {
        this->applicationDirPath = oCacic.getCacicMainFolder();
    }

    QDir dir(oCacic.getCacicMainFolder());
    if (!dir.exists()){
        oCacic.createFolder(oCacic.getCacicMainFolder());
    }
    logManager = QLogger::QLoggerManager::getInstance();
    logManager->addDestination(oCacic.getCacicMainFolder() + "/Logs/cacic.log","Install Cacic",QLogger::InfoLevel);
    logManager->addDestination(oCacic.getCacicMainFolder() + "/Logs/cacic.log","Install Cacic",QLogger::ErrorLevel);
}

InstallCacic::~InstallCacic()
{
}

void InstallCacic::run(QStringList argv, int argc) {

    // TODO: Verificar hash no gerente.
    oCacicComm = new CacicComm();

    bool ok;
    //valida os parametros repassados
    QMap<QString, QString> param = validaParametros(argv, argc, &ok);

    //se tiver usuario, senha e url e nenhum parâmetro a mais.
    if (ok){
        //inicia a instalação.
        this->install();
    } else if ((param.contains("default")) && (param["default"] == "uninstall")){
        //Se tiver -uninstall como parâmetro, inicia desinstalação.
        QLogger::QLog_Info("Install Cacic", "Desinstalando cacic!");
        this->uninstall();
    } else if ((param.contains("default")) && (param["default"] == "configure")) {
        //Se tiver -configure, inicia configuração (trocar de host, usuário ou senha)
        QLogger::QLog_Info("Install Cacic", QString("Configuração do agente."));
        configurar(param);
    } else if ((param.contains("default")) && (param["default"] == "updateService")) {
        //Se tiver -updateService, verifica a pasta temporária se há algum módulo para update.
        QLogger::QLog_Info("Install Cacic", "Atualizando cacic!");
        updateService();
    } else {
        parametrosIncorretos();
    }
#ifdef Q_OS_WIN
//    system("PAUSE");
#endif
    logManager->closeLogger();
    logManager->wait();
    emit finished();
}

//---------MÉTODOS-----------

void InstallCacic::parametrosIncorretos(){
    std::cout << "\nInstalador do Agente Cacic.\n\n"
              << "Parametros incorretos/incompletos. (<obrigatorios> [opcional])\n\n"
              << "Instalação: <-host=url_gerente> <-user=usuario> <-pass=senha>\n"
              << "Desinstalar: <-uninstall>\n"
              << "Configurar: <-configure> [-user=usuario] [-pass=senha] [-host=url_gerente]\n\n"
              << "  -host=url_gerente       url_gerente: Caminho para a aplicação do gerente.\n"
              << "  -user=usuario           usuario: usuário de login no gerente.\n"
              << "  -pass=senha             senha: senha de login no gerente\n";
}

/*---------------------------------------------------------------------------------------------------
 * Se tiver -updateService, verifica a pasta temporária se há algum módulo para update.
 * --------------------------------------------------------------------------------------------------
 */
void InstallCacic::updateService()
{
#ifdef Q_OS_WIN
    bool ok;
#endif
    bool serviceUpdate = false;
    QLogger::QLog_Info("Install Cacic", "Verificando a existência de módulos na pasta temporária.");
    QDir dir(oCacic.getCacicMainFolder() + "/temp");
    dir.setFilter(QDir::Files | QDir::Hidden | QDir::NoSymLinks | QDir::Executable);
    dir.setSorting(QDir::Size | QDir::Reversed);

    QFileInfoList list = dir.entryInfoList();
    for (int i = 0; i<list.size(); i++){
        if(!(list.at(i).fileName().contains("install-cacic"))){
            QLogger::QLog_Info("Install Cacic", "Módulo \"" + list.at(i).filePath() + "\" encontrado para atualização.");
            if((list.at(i).fileName().contains("cacic-service"))){
                serviceUpdate = true;
                QLogger::QLog_Info("Install Cacic", "Parando serviço para atualização.");
#ifdef Q_OS_WIN
                bool ok = false;
                system("start /b sc stop cacicdaemon");
#else
                ConsoleObject console;
                console("/etc/init.d/cacic3 stop");
#endif
            }
            QFile novoModulo(list.at(i).filePath());
            if (QFile::exists(applicationDirPath + "/" + list.at(i).fileName())){
                QLogger::QLog_Info("Install Cacic", "Excluindo versão antiga de "+list.at(i).fileName());
                if (!QFile::remove(applicationDirPath + "/" + list.at(i).fileName()))
                    QLogger::QLog_Info("Install Cacic", "Falha ao excluir "+list.at(i).fileName());
            }
            //Nova verificação pra ter certeza de que não existe, porque se existir ele não vai copiar.
            if (!QFile::exists(applicationDirPath + "/" + list.at(i).fileName())){
                novoModulo.copy(applicationDirPath + "/" + list.at(i).fileName());
                QLogger::QLog_Info("Install Cacic", "Copiando arquivo para " + applicationDirPath);
            } else {
                QLogger::QLog_Info("Install Cacic", "Falha ao excluir " + list.at(i).filePath());
            }

            if (!novoModulo.remove())
                QLogger::QLog_Info("Install Cacic", "Falha ao excluir "+list.at(i).fileName()+" da pasta temporária.");
        }
    }
    if (serviceUpdate){
        QLogger::QLog_Info("Install Cacic", "Iniciando o serviço cacic.");
#ifdef Q_OS_WIN
        QProcess proc;
        proc.setWorkingDirectory(oCacic.getCacicMainFolder());
        proc.execute(oCacic.getCacicMainFolder() + "/cacic-service.exe", QStringList("-start"));

        if(!ok) {
            QLogger::QLog_Info("Install Cacic", "Falha ao iniciar serviço");
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
            QLogger::QLog_Info("Install Cacic", QString("Host alterado para " + param["host"]));
            std::cout << "Url alterada para \"" << param["host"].toStdString() << "\"\n";
        }
        if (param.contains("user")){
            reg["usuario"] = param["user"];
            QLogger::QLog_Info("Install Cacic", QString("Usuário alterado para " + param["user"]));
            std::cout << "Usuário alterado para \"" << param["user"].toStdString() << "\"\n";
        }
        if (param.contains("pass")){
            reg["senha"] = param["pass"];
            QLogger::QLog_Info("Install Cacic", QString("Senha alterada."));
            std::cout << "Senha alterada.\n";
        }
        if (reg.size() > 0) {
            oCacic.setValueToRegistry("Lightbase", "Cacic", reg);
            std::cout << "\nRegistro atualizado.\n\n";
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
    QLogger::QLog_Info("Install Cacic", QString("Inicio de instalacao"));
    std::cout << " - - INSTALL CACIC - -\n";
    oCacicComm->setUrlGerente(this->argumentos["host"]);
    oCacicComm->setUsuario(this->argumentos["user"]);
    oCacicComm->setPassword(this->argumentos["pass"]);
    std::cout << "Realizando login...\n";
    QLogger::QLog_Info("Install Cacic", "Realizando login...");
    QJsonObject jsonLogin = oCacicComm->login(&ok);
    if (ok){
        std::cout << "Login realizado com sucesso...\n";
        QLogger::QLog_Info("Install Cacic", "Login realizado com sucesso!");
        QJsonObject jsonComm;
        QLogger::QLog_Debug("Install", "Login: " + jsonLogin["reply"].toObject()["chavecript"].toString());
        //conectado, grava a chave na classe;
        oCacic.setChaveCrypt(jsonLogin["reply"].toObject()["chavecript"].toString());

        jsonComm["computador"] = oCacicComputer.toJsonObject();
        std::cout << "Pegando informações do gerente...\n";
        QLogger::QLog_Info("Install Cacic", "Pegando informações do gerente...");
        QJsonObject configs = oCacicComm->comm("/ws/neo/config", &ok, jsonComm, true);
        if (ok){
            QJsonObject configsJson = configs["reply"].toObject()["agentcomputer"].toObject();
            oCacicComm->setUrlGerente(configsJson["applicationUrl"].toString());
#ifdef Q_OS_WIN
            oCacic.setCacicMainFolder("c:/cacic");
#elif defined(Q_OS_LINUX)
            oCacic.setCacicMainFolder("/usr/share/cacic");
#endif

            oCacic.createFolder(oCacic.getCacicMainFolder());
            //grava chave em registro;
            QVariantMap registro;
            registro["key"] = oCacic.getChaveCrypt();
            registro["password"] = oCacicComm->getPassword();
            registro["usuario"] = oCacicComm->getUsuario();
            registro["mainFolder"] = oCacic.getCacicMainFolder();
            registro["applicationUrl"] = oCacicComm->getUrlGerente();
            oCacic.setValueToRegistry("Lightbase", "Cacic", registro);
            std::cout << "Sucesso, salvando configurações em arquivo...\n";
            QLogger::QLog_Info("Install Cacic", "Sucesso! Salvando configurações em arquivo...");
            oCacic.setJsonToFile(configs["reply"].toObject(), oCacic.getCacicMainFolder() + "/getConfig.json");
            //starta o processo do cacic.

            QJsonObject metodoDownload;
            metodoDownload = configsJson["metodoDownload"].toObject();
            oCacicComm->setFtpPass(metodoDownload["senha"].toString());
            oCacicComm->setFtpUser(metodoDownload["usuario"].toString());
            std::cout << "Realizando download do serviço...\n";
            QLogger::QLog_Info("Install Cacic", "Realizando download do serviço...");
            std::cout << "Baixando serviço...\n";
#ifdef Q_OS_WIN
            oCacicComm->fileDownload(metodoDownload["tipo"].toString(),
                    metodoDownload["url"].toString(),
                    metodoDownload["path"].toString() + (!metodoDownload["path"].toString().endsWith("/") ? "/" : "") +
                    "cacic-service.exe",
                    oCacic.getCacicMainFolder());

            QFile fileService(oCacic.getCacicMainFolder()+"/cacic-service.exe");
            if ((!fileService.exists() || !fileService.size() > 0)) {
                std::cout << "Falha ao baixar arquivo.\n";
                Logger::QLog_Info("Install Cacic", "Falha ao baixar o serviço...");
                this->uninstall();
                fileService.close();
            } else {
                QProcess proc;
                proc.setWorkingDirectory(oCacic.getCacicMainFolder());
                std::cout << "Executando instalador do serviço...\n";
                Logger::QLog_Info("Install Cacic", "Executando instalador do serviço...");
                proc.execute(oCacic.getCacicMainFolder() + "/cacic-service.exe", QStringList("-install"));
                std::cout << "Iniciando serviço...\n";
                Logger::QLog_Info("Install Cacic", "Iniciando serviço...");

                system("sc config cacicdaemon start= delayed-auto");
                system("sc failure cacicdaemon reset= 60 actions= restart/60000");
                system("sc start cacicdaemon");

                if (proc.exitStatus() != QProcess::NormalExit) {
                    std::cout << "Erro ao executar serviço para instalação: " << proc.errorString().toStdString() << "\n"
                              << "Verifique se o serviço foi instalado e tente iniciá-lo a mão.\n";
                    ok = false;
    //                uninstall();
                } else {
                    std::cout << "Instalação realizada com sucesso." << "\n";
                    QLogger::QLog_Info("Install Cacic", QString("Instalação realizada com sucesso."));
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
                std::cout << "Falha ao baixar arquivo.\n";
                QLogger::QLog_Info("Install Cacic", "Falha ao baixar o serviço...");
                this->uninstall();
                fileService.close();
            } else {
                std::cout << "Iniciando serviço...\n";
                ConsoleObject console;
                std::cout << console("/etc/init.d/cacic3 start").toStdString();
            }

#endif          

        } else {
            std::cout << "Falha ao pegar configurações: " << configs["error"].toString().toStdString() << "\n";
            QLogger::QLog_Info("Install Cacic", QString("Falha ao pegar configurações: ") + configs["error"].toString());
        }

    } else {
        std::cout << "Nao foi possivel realizar o login.\n"
                  << "  Código: " << jsonLogin["codestatus"].toString().toStdString() << "\n"
                  << "  " << jsonLogin["error"].toString().toStdString() << "\n";
        QLogger::QLog_Info("Install Cacic", QString("Falha no login: ") + jsonLogin["error"].toString());
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
    bool ok;
#ifdef Q_OS_WIN
    //TODO: PARAR O SERVIÇO no windows
    QProcess proc;
    proc.setWorkingDirectory(oCacic.getCacicMainFolder());
    proc.execute(oCacic.getCacicMainFolder() + "/cacic-service.exe", QStringList("-stop"));
    proc.execute(oCacic.getCacicMainFolder() + "/cacic-service.exe", QStringList("-uninstall"));

#elif defined(Q_OS_LINUX)
    ConsoleObject console;
    QStringList outputColumns;
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
                    QLogger::QLog_Info("Install Cacic", QString("Cacic-service interrompido."));
                }
            }
        }
    }
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

    std::cout << "\nCacic desinstalado com sucesso.\n";
}

QMap<QString, QString> InstallCacic::getArgumentos()
{
    return argumentos;
}

void InstallCacic::setArgumentos(QMap<QString, QString> value)
{
    this->argumentos = value;
}


