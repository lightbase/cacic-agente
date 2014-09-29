#include "installcacic.h"

InstallCacic::InstallCacic(QObject *parent) :
    QObject(parent)
{
    logManager = QLogger::QLoggerManager::getInstance();
    logManager->addDestination("./install.log","Install",QLogger::DebugLevel);
}

InstallCacic::~InstallCacic()
{
    logManager->closeLogger();
}

void InstallCacic::run(QStringList argv, int argc) {

    QLogger::QLog_Debug("Install", "Inicio de instalacao");

    oCacicComm = new CacicComm();

    bool ok;
    //valida os parametros repassados
    QMap<QString, QString> param = validaParametros(argv, argc, &ok);
    //se tiver usuario, senha e url
    if (ok){
        oCacicComm->setUrlGerente(this->argumentos["host"]);
        oCacicComm->setUsuario(this->argumentos["user"]);
        oCacicComm->setPassword(this->argumentos["password"]);
        QJsonObject jsonLogin = oCacicComm->login(&ok);
        if (ok){
            QJsonObject jsonComm;
            QLogger::QLog_Debug("Install", "Login: " + jsonLogin["reply"].toObject()["chavecript"].toString());
            //conectado, grava a chave na classe;
            oCacic.setChaveCrypt(jsonLogin["reply"].toObject()["chavecrip"].toString());
            jsonComm["computador"] = oCacicComputer.toJsonObject();
            QJsonObject configs = oCacicComm->comm("/ws/neo/config", &ok, jsonComm);
            qDebug () << configs;
            if (ok){
                oCacicComm->setUrlGerente(configs["reply"].toObject()["applicationUrl"].toString());
#ifdef Q_OS_WIN
                oCacic.setCacicMainFolder(configs["reply"].toObject()["cacic_main_folder"].isString() ?
                                          configs["reply"].toObject()["cacic_main_folder"].toString() :
                                          "c:/cacic/");
#elif defined(Q_OS_LINUX)
                oCacic.setCacicMainFolder(configs["reply"].toObject()["cacic_main_folder"].isString() ?
                                          configs["reply"].toObject()["cacic_main_folder"].toString() :
                                          "/usr/cacic");
#endif

                oCacic.createFolder(oCacic.getCacicMainFolder());
                //grava chave em registro;
                QVariantMap registro;
                registro["key"] = oCacic.getChaveCrypt();
                registro["mainFolder"] = oCacic.getCacicMainFolder();
                oCacic.setValueToRegistry("Lightbase", "Cacic", registro);
                //starta o processo do cacic.

                //TO DO: Fazer download do serviço
    #ifdef Q_OS_WIN
                oCacicComm->ftpDownload("agentes/cacic.exe");
                QString exitStatus = oCacic.startProcess(oCacic.getCacicMainFolder() + "cacic.exe", false, &ok, QStringList("-install"));
    #else
                oCacicComm->ftpDownload("agentes/cacic");
                QString exitStatus = oCacic.startProcess(oCacic.getCacicMainFolder() + "cacic", false, &ok, QStringList("-install"));
    #endif
                if (!ok)
                    std::cout << "Erro ao iniciar o processo: "
                              << exitStatus.toStdString() << "\n";
                else {
                    std::cout << "Instalação realizada com sucesso.";
                }
            } else {
                std::cout << "Falha ao pegar configurações: " << configs["error"].toString();
            }

        } else
            std::cout << "Nao foi possivel realizar o login.\n  "
                      << jsonLogin["error"].toString().toStdString();
    } else if ((param.contains("default")) && (param["default"] == "uninstall")){
        oCacic.deleteFolder("c:/cacic");
        oCacic.removeRegistry("Lightbase", "Cacic");
        std::cout << "Cacic desinstalado com sucesso.\n";
    } else {
        std::cout << "\nInstalador do Agente Cacic.\n\n"
                  << "Parametros incorretos. (<obrigatorios> [opcional])\n\n"
                  << "<-host=url_gerente> <-user=usuario> <-password=senha> [-help]\n\n"
                  << "  <-host=url_gerente>       url_gerente: Caminho para a aplicação do gerente.\n"
                  << "  <-user=usuario>           usuario: usuário de login no gerente.\n"
                  << "  <-password=senha>         senha: senha de login no gerente\n"
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
    *ok = (bool) map.contains("host") && map.contains("user") && map.contains("password");
    if (*ok){
        this->argumentos = map;
    }
    return map;
}

QMap<QString, QString> InstallCacic::getArgumentos()
{
    return argumentos;
}

void InstallCacic::setArgumentos(QMap<QString, QString> value)
{
    this->argumentos = value;
}


