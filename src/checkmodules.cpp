#include "checkmodules.h"

CheckModules::CheckModules(const QString &workingPath, const QString &workingModule)
{
    logcacic = new LogCacic(LOG_CHECKMODULES, workingPath+"/Logs");
    cacicMainFolder = workingPath;
    QDir tempPath(cacicMainFolder + "/temp");
    if (!tempPath.exists()){
        tempPath.mkdir(cacicMainFolder + "/temp");
    }
}

CheckModules::~CheckModules()
{
    logcacic->~LogCacic();
    delete logcacic;
}

CModuleArray CheckModules::getModules() const {
    return modules;
}

/*********************************************************************************
 * Verificar os módulos instalado de acordo com os hashs locais e do gerente,
 * se houver diferença entre eles, baixa o novo do servidor e salva na pasta temp.
 *********************************************************************************/
bool CheckModules::start(){
    bool ok = true;
    QJsonObject configFile;
    if(!QFile::exists(cacicMainFolder + "/getConfig.json")){
        logcacic->escrever(LogCacic::ErrorLevel, "Arquivo de configuração inexistente.");
        return false;
    }
    configFile = CCacic::getJsonFromFile(cacicMainFolder + "/getConfig.json");
    if (!configFile.isEmpty()) {
        //pega url do gerente.
        this->applicationUrl = configFile["agentcomputer"].toObject()
                               ["metodoDownload"].toObject()["url"].toString();

        QJsonArray modulos;
        //pega o jsonarray dos módulos
        modulos = configFile["agentcomputer"].toObject()["modulos"].toObject()
                  ["cacic"].toArray();
        if (!modules.loadFromJsonArray(modulos) == 0){
            logcacic->escrever(LogCacic::InfoLevel,
                               "Falha ao recuperar informações dos módulos do arquivo de configuração.");
            logcacic->escrever(LogCacic::ErrorLevel,
                               "Erro ao recuperar informações do json.\n\tCodigo: " +
                               QString::number(modules.getError()));
            return false;
        }
    } else {
        logcacic->escrever(LogCacic::ErrorLevel,
                           QString("Erro ao pegar informações do arquivo " +
                                   cacicMainFolder + "/getConfig.json"));
    }
    if (modules.size() > 0){
        for(int i = 0; i<modules.size(); i++){
            ok = (this->verificaModulo(modules[i].name(), modules[i].hash(),
                                       modules[i].urlDownload()) && ok);
        }
    } else {
        logcacic->escrever(LogCacic::InfoLevel, QString("Não há modulo a ser verificado."));
    }

    return ok;
}

/*******************************************************
 * Método que vai verificar cada módulo individualmente.
 *******************************************************/
bool CheckModules::verificaModulo(const QString &moduloName, const QString &moduloHash, const QString &moduloUrl = QString())
{
    QFile *modulo, *moduloTemp;
    bool downloadOk = false;
    //pega o arquivo do módulo selecionado
#ifdef Q_OS_WIN
    if (moduloName == "install-cacic.exe")
        modulo = new QFile(cacicMainFolder + "/bin/" + moduloName);
    else
        modulo = new QFile(cacicMainFolder + "/" + moduloName);
#else
    modulo = new QFile(cacicMainFolder + "/" + moduloName);
#endif
    moduloTemp = new QFile(cacicMainFolder + "/temp/" + moduloName);
    if (modulo->exists()) {
        if (!modulo->open(QFile::ReadOnly)) {
            logcacic->escrever(LogCacic::ErrorLevel, "#2 Falha ao tentar recuperar hash do módulo atual");
            return false;
        }
    }
    QString filePath = modulo->exists() ? cacicMainFolder + "/temp/":
                                          cacicMainFolder + "/";
    //verifica se o módulo não existe e se o tamaho não é maior que 1 byte ou se o hash é diferente ao informado pelo json
    if ((!modulo->exists() || !CCacic::Md5IsEqual(modulo->readAll(), moduloHash))) {
        modulo->close();
        logcacic->escrever(LogCacic::InfoLevel, QString("Atualização de " + moduloName + " necessária."));
        logcacic->escrever(LogCacic::InfoLevel, "Baixando em: " + filePath);
        QFile *novoModulo;
        QJsonObject metodoDownload;
        //verifica o tipo de download e tenta baixar o módulo para a pasta temporária.
        metodoDownload = CCacic::getJsonFromFile(cacicMainFolder + "/getConfig.json")
                ["agentcomputer"].toObject()
                ["metodoDownload"].toObject();
        if (!metodoDownload.isEmpty()){
            //verifica se já possuía o módulo atualizado na pasta temporária, se não baixa um novo.
            if (moduloTemp->exists()) {
                if (!moduloTemp->open(QFile::ReadOnly)){
                    logcacic->escrever(LogCacic::ErrorLevel, "#3 Falha ao tentar recuperar hash do módulo da pasta temporária.");
                    return false;
                }
            }
            if ((!moduloTemp->exists() || !CCacic::Md5IsEqual(moduloTemp->readAll(), moduloHash))){
                moduloTemp->close();
                CacicComm *oCacicComm = new CacicComm(LOG_CHECKMODULES, this->cacicMainFolder);

                if(metodoDownload.contains("tipo")
                        && metodoDownload["tipo"].isString()
                        && metodoDownload["tipo"].toString() == "ftp") {

                    if (!metodoDownload["usuario"].isNull())
                        oCacicComm->setFtpUser(metodoDownload["usuario"].toString());
                    if (!metodoDownload["senha"].isNull())
                        oCacicComm->setFtpPass(metodoDownload["senha"].toString());
                }

                if (!moduloUrl.isEmpty() && !moduloUrl.isNull()){
                    QUrl url(moduloUrl);
                    if(url.isValid()){
                        downloadOk = oCacicComm->fileDownload(url.scheme(),
                                                              url.authority(),
                                                              url.path(),
                                                              filePath);
                    } else {
                        logcacic->escrever(LogCacic::InfoLevel, "Falha durante download do " +
                                                                moduloName + ".");
                        logcacic->escrever(LogCacic::ErrorLevel, "Url de download do módulo " +
                                                                moduloName + " inválida.");
                    }
                } else {
                    downloadOk = oCacicComm->fileDownload(metodoDownload["tipo"].toString(),
                                                this->applicationUrl,
                                                metodoDownload["path"].toString() +
                                                (metodoDownload["path"].toString().endsWith("/") ?
                                                    moduloName : "/" + moduloName),
                                                filePath);
                }
            } else {
                moduloTemp->close();
                return true;
            }
            novoModulo = new QFile(filePath + moduloName);
            if (downloadOk){
                //faz uma verificação do novo módulo.
                if (!(novoModulo->exists() && novoModulo->size()>1)){
                    logcacic->escrever(LogCacic::ErrorLevel,
                                       QString("#4 Falha ao baixar " + moduloName +
                                               "("+metodoDownload["tipo"].toString()+ "://" +
                                       this->applicationUrl + metodoDownload["path"].toString() +
                            (metodoDownload["path"].toString().endsWith("/") ? moduloName : "/" + moduloName)+")"));
                    novoModulo->remove();
                    return false;
                } else {
                    logcacic->escrever(LogCacic::InfoLevel, QString(moduloName + " baixado com sucesso!"));
                    return true;
                }
            } else {
                logcacic->escrever(LogCacic::ErrorLevel, QString("#5 Problemas durante o download de " + moduloName));
                return false;
            }
        } else {
            logcacic->escrever(LogCacic::ErrorLevel, QString("#6 Não foi possível recuperar json de " +
                                                             cacicMainFolder + "/getConfig.json ao tentar baixar " +
                                                             moduloName));
            return false;
        }
    } else {
        modulo->close();
        return true;
    }
}
