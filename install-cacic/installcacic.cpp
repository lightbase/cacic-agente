#include "installcacic.h"

InstallCacic::InstallCacic(QObject *parent) :
    QObject(parent)
{

}

void InstallCacic::run(QStringList argv, int argc) {
    bool ok;
    //valida os parametros repassados
    validaParametros(argv, argc, &ok);
    //se tiver usuario, senha e url
    if (ok){
        oCacicComm.setUrlGerente(this->argumentos["host"]);
        oCacicComm.setUsuario(this->argumentos["user"]);
        oCacicComm.setPassword(this->argumentos["password"]);
        QJsonObject jsonLogin = oCacicComm.login(&ok);
        if (ok){
            //conectado, grava a chave na classe;
            oCacic.setChaveCrypt(jsonLogin["reply"].toObject()["chavecrip"].toString());
            oCacic.setCacicMainFolder("c:/cacic");
            oCacic.createFolder(oCacic.getCacicMainFolder());
            //grava chave em arquivo json;
            QJsonObject configJson = oCacic.getJsonFromFile(oCacic.getCacicMainFolder() + "/cacicTeste.json");
            QJsonObject configToSave = configJson["configs"].toObject();
            configToSave["chaveCrypt"] = QJsonValue::fromVariant(oCacic.getChaveCrypt());
            configJson["configs"] = configToSave;
            oCacic.setJsonToFile(configJson, oCacic.getCacicMainFolder() + "/cacicTeste.json");

            //starta o processo do cacic.
#ifdef Q_OS_WIN
            QString exitStatus = oCacic.startProcess("cacic.exe", true, &ok);
            if (!ok)
                std::cout << "Erro ao iniciar o processo: "
                          << exitStatus.toStdString() << "\n";
#else
            oCacic.startProcess("cacic.exe", true, &ok);
            if (!ok)
                qDebug() << "Erro ao iniciar o processo.";
#endif
        }
        else
            std::cout << "Nao foi possivel realizar o login.\n  "
                      << jsonLogin["error"].toString().toStdString();
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


