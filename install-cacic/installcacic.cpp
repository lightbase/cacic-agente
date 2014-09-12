#include "installcacic.h"

InstallCacic::InstallCacic(QObject *parent) :
    QObject(parent)
{
    logManager = QLogger::QLoggerManager::getInstance();
    logManager->addDestination("./log.txt","Install",QLogger::DebugLevel);
}

void InstallCacic::run(QStringList argv, int argc) {

    QLogger::QLog_Debug("Install", "Inicio de instalacao");
    bool ok;
    //valida os parametros repassados
    QMap<QString, QString> param = validaParametros(argv, argc, &ok);
    //se tiver usuario, senha e url
    if (ok){
        oCacicComm.setUrlGerente(this->argumentos["host"]);
        oCacicComm.setUsuario(this->argumentos["user"]);
        oCacicComm.setPassword(this->argumentos["password"]);
        QJsonObject jsonLogin = oCacicComm.login(&ok);
        if (ok){
            //conectado, grava a chave na classe;
            oCacic.setChaveCrypt(jsonLogin["reply"].toObject()["chavecrip"].toString());
#ifdef Q_OS_WIN
            oCacic.setCacicMainFolder("c:/cacic/");
#elif defined(Q_OS_LINUX)
            oCacic.setCacicMainFolder("/home/cacic");
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
            QString exitStatus = oCacic.startProcess(oCacic.getCacicMainFolder() + "cacic.exe", false, &ok);
#else
            QString exitStatus = oCacic.startProcess(oCacic.getCacicMainFolder() + "cacic", false, &ok);
#endif
            if (!ok)
                std::cout << "Erro ao iniciar o processo: "
                          << exitStatus.toStdString() << "\n";
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

    logManager->closeLogger();
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


