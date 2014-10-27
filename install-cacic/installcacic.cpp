#include "installcacic.h"

InstallCacic::InstallCacic(QObject *parent) :
    QObject(parent)
{
    QDir dir;
    this->applicationDirPath = dir.currentPath();

    logManager = QLogger::QLoggerManager::getInstance();
    logManager->addDestination(this->applicationDirPath + "/Logs/cacic.log","Install Cacic",QLogger::InfoLevel);
    logManager->addDestination(this->applicationDirPath + "/Logs/cacic.log","Install Cacic",QLogger::ErrorLevel);
}

InstallCacic::~InstallCacic()
{
    logManager->closeLogger();
    delete logManager;
}

void InstallCacic::run(QStringList argv, int argc) {

    // TODO: Verificar hash no gerente.

    QLogger::QLog_Info("Install Cacic", QString("Inicio de instalacao"));

    oCacicComm = new CacicComm();

    bool ok;
    //valida os parametros repassados
    QMap<QString, QString> param = validaParametros(argv, argc, &ok);
    //se tiver usuario, senha e url
    if (ok){
        oCacicComm->setUrlGerente(this->argumentos["host"]);
        oCacicComm->setUsuario(this->argumentos["user"]);
        oCacicComm->setPassword(this->argumentos["pass"]);
        QJsonObject jsonLogin = oCacicComm->login(&ok);
        if (ok){
            QJsonObject jsonComm;
            QLogger::QLog_Debug("Install", "Login: " + jsonLogin["reply"].toObject()["chavecript"].toString());
            //conectado, grava a chave na classe;
            oCacic.setChaveCrypt(jsonLogin["reply"].toObject()["chavecrip"].toString());

            jsonComm["computador"] = oCacicComputer.toJsonObject();
            QJsonObject configs = oCacicComm->comm("/ws/neo/config", &ok, jsonComm);
            if (ok){
                QJsonObject configsJson = configs["reply"].toObject()["agentcomputer"].toObject();
                oCacicComm->setUrlGerente(configsJson["applicationUrl"].toString());
#ifdef Q_OS_WIN
                oCacic.setCacicMainFolder(configsJson["cacic_main_folder"].isString() ?
                            configsJson["cacic_main_folder"].toString() :
                    "c:/cacic/");
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

                oCacic.setJsonToFile(configs["reply"].toObject(), oCacic.getCacicMainFolder() + "/getConfig.json");
                //starta o processo do cacic.

                //TO DO: Fazer download do serviço
                QJsonObject metodoDownload;
                metodoDownload = configsJson["metodoDownload"].toObject();
                oCacicComm->setFtpPass(metodoDownload["senha"].toString());
                oCacicComm->setFtpUser(metodoDownload["usuario"].toString());
#ifdef Q_OS_WIN
                oCacicComm->fileDownload(metodoDownload["tipo"].toString(),
                        metodoDownload["url"].toString(),
                        metodoDownload["path"].toString() + "/cacic-service.exe",
                        oCacic.getCacicMainFolder());

                QString exitStatus = oCacic.startProcess(oCacic.getCacicMainFolder() + "/cacic-service.exe",
                                                         false,
                                                         &ok,
                                                         QStringList("-install");
#else

                oCacicComm->fileDownload(metodoDownload["tipo"].toString(),
                        metodoDownload["url"].toString(),
                        metodoDownload["path"].toString() + "cacic-service",
                        oCacic.getCacicMainFolder());

                QFile fileService(oCacic.getCacicMainFolder()+"/cacic-service");
                if ((!fileService.exists() || !fileService.size() > 0)) {
                    std::cout << "Falha ao baixar arquivo.\n";
                    this->uninstall();
                    return;
                }

                fileService.close();
                ConsoleObject console;
                std::cout << console("/etc/init.d/cacic3 start").toStdString();

#endif
                if (!ok) {
                    QLogger::QLog_Info("Install Cacic", QString("Erro ao iniciar o processo"));
                } else {
                    std::cout << "Instalação realizada com sucesso." << "\n";
                    QLogger::QLog_Info("Install Cacic", QString("Instalação realizada com sucesso."));
                }

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
    } else if ((param.contains("default")) && (param["default"] == "uninstall")){
        this->uninstall();
    } else {
        std::cout << "\nInstalador do Agente Cacic.\n\n"
                  << "Parametros incorretos. (<obrigatorios> [opcional])\n\n"
                  << "<-host=url_gerente> <-user=usuario> <-pass=senha> [-help]\n\n"
                  << "  <-host=url_gerente>       url_gerente: Caminho para a aplicação do gerente.\n"
                  << "  <-user=usuario>           usuario: usuário de login no gerente.\n"
                  << "  <-pass=senha>             senha: senha de login no gerente\n"
                  << "  [-help]                   Lista todos comandos.\n";
    }


    emit finished();
}

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
    *ok = (bool) map.contains("host") && map.contains("user") && map.contains("pass");
    if (*ok){
        this->argumentos = map;
    }
    return map;
}

void InstallCacic::uninstall()
{
    //TODO: PARAR O SERVIÇO no windows

#if defined(Q_OS_LINUX)
    ConsoleObject console;
    QStringList outputColumns;

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

    oCacic.deleteFolder(oCacic.getCacicMainFolder());
    oCacic.removeRegistry("Lightbase", "Cacic");
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


