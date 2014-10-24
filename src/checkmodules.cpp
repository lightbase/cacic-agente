#include "checkmodules.h"

CheckModules::CheckModules(const QString &workingPath)
{

    logManager = QLogger::QLoggerManager::getInstance();
    logManager->addDestination(workingPath + "/Logs/cacicLog.txt","CheckModules",QLogger::InfoLevel);
    logManager->addDestination(workingPath + "/Logs/cacicLog.txt","CheckModules",QLogger::ErrorLevel);

    oCacic.setCacicMainFolder(workingPath);
    QJsonObject configFile;
    configFile = oCacic.getJsonFromFile(workingPath + "/getConfig.json");
    if (!configFile.isEmpty()) {
        //pega url do gerente.
        this->applicationUrl = configFile["agentcomputer"].toObject()["metodoDownload"].toObject()["url"].toString();

        QJsonArray modulos;
        //pega o jsonarray dos módulos
        modulos = configFile["agentcomputer"].toObject()["modulos"].toObject()["cacic"].toArray();
        foreach (QJsonValue modulo, modulos){
            //grava o nome com o hash de cada modulo
            modules[modulo.toObject()["nome"].toString()] = modulo.toObject()["hash"].toString();
        }
    }
    //TODO: Completar constructor. Agora que tenho os nomes dos módulos e os hashs, fazer a verificação.

    QDir tempPath(oCacic.getCacicMainFolder() + "/temp");
    if (!tempPath.exists()){
        tempPath.mkdir(oCacic.getCacicMainFolder() + "/temp");
    }
}

CheckModules::~CheckModules()
{
    logManager->closeLogger();
    delete logManager;
}

QVariantMap CheckModules::getModules() const {
    return modules;
}

bool CheckModules::start(){
    bool ok = true;
    if (!modules.isEmpty()){
        QVariantMap::const_iterator i = modules.constBegin();
        do {
//            qDebug() << "Módulo: " << i.key() << " | Hash: " << i.value().toString();
            ok = (this->verificaModulo(i.key(), i.value().toString()) && ok);
            i++;
        } while (i != modules.constEnd());
    } else {
        //força download do gercols.
        return this->verificaModulo("gercols", "gtgytt");
    }

    return ok;
}

bool CheckModules::verificaModulo(const QString &moduloName, const QString &moduloHash)
{
    QFile *modulo;
    bool downloadOk = false;
    //pega o arquivo do módulo selecionado
    modulo = new QFile(oCacic.getCacicMainFolder() + "/" + moduloName);

    //verifica se o módulo existe, se o tamaho é maior que 1 byte e se o hash é igual ao informado pelo json
    if (!(modulo->exists() && modulo->size()>1 && oCacic.Md5IsEqual(QVariant::fromValue(modulo), moduloHash))){
        QFile *novoModulo;
        QJsonObject metodoDownload;
        //verifica o tipo de download e tenta baixar o módulo para a pasta temporária.
        metodoDownload = oCacic.getJsonFromFile("getConfig.json")["agentcomputer"].toObject()["metodoDownload"].toObject();
        oCacicComm.setFtpUser(metodoDownload["usuario"].toString());
        oCacicComm.setFtpPass(metodoDownload["senha"].toString());

        downloadOk = oCacicComm.fileDownload(metodoDownload["tipo"].toString(),
                                            this->applicationUrl,
                                            metodoDownload["path"].toString() + moduloName,
                                            oCacic.getCacicMainFolder() + "/temp");

        novoModulo = new QFile(oCacic.getCacicMainFolder() + "/temp/" + moduloName);
        if (downloadOk){
            //faz uma verificação do novo módulo.
            if (!(novoModulo->exists() && novoModulo->size()>1)){
                QLogger::QLog_Error("CheckModules", moduloName + "falha no download..");
                novoModulo->remove();
                return false;
            } else {
                QLogger::QLog_Info("CheckModules", moduloName + "Sucesso!");
                return true;
            }
        } else {
            return false;
        }

    }
}
