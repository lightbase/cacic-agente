#include "cacicthread.h"

CacicThread::CacicThread(QString applicationDirPath)
{
    this->applicationDirPath = applicationDirPath;
    iniciarInstancias();
}

void CacicThread::run(){
    iniciarModulo();
}

void CacicThread::setModuloDirPath(const QString &value)
{
    moduloDirPath = value;
}

void CacicThread::iniciarModulo()
{
    registraInicioColeta();
    if (QFile::exists(this->moduloDirPath)){
        QDir::setCurrent(this->applicationDirPath);
        QProcess proc;
        proc.setWorkingDirectory(this->applicationDirPath);
        proc.execute(this->moduloDirPath);
        if((proc.atEnd()) && (proc.exitStatus() == QProcess::NormalExit)){
            registraFimColeta(true);
            if(enviarColeta()){
                QLogger::QLog_Info(Identificadores::LOG_DAEMON_THREAD, QString("Coleta enviada com sucesso."));
            }else{
                QLogger::QLog_Info(Identificadores::LOG_DAEMON_THREAD, QString("Coleta não enviada."));
            }
        }else{
            if((!proc.atEnd()) || (proc.exitStatus() == QProcess::CrashExit)){
                registraFimColeta(false);
                proc.kill();
            }
        }
        proc.close();
    } else {
        QLogger::QLog_Error(Identificadores::LOG_DAEMON_THREAD, QString("Módulo inexistente."));
    }
    cMutex->unlock();
    QLogger::QLog_Info(Identificadores::LOG_DAEMON_THREAD, QString("Semáforo aberto com sucesso."));
}

void CacicThread::setOCacicComm(CacicComm *value)
{
    OCacicComm = value;
}

void CacicThread::setCcacic(CCacic *value)
{
    ccacic = value;
}

void CacicThread::setNomeModulo(const QString &value)
{
    nomeModulo = value;
}

bool CacicThread::verificaForcarColeta(){
    QJsonObject agenteConfigJson;
    QJsonObject configuracoes;
    QJsonObject result = ccacic->getJsonFromFile(this->applicationDirPath + "/getConfig.json");
    if(!result.contains("error") && !result.isEmpty()){
        agenteConfigJson = result["agentcomputer"].toObject();
        configuracoes = agenteConfigJson["configuracoes"].toObject();
        if(!configuracoes["nu_forca_coleta"].isNull()){
            if(configuracoes["nu_forca_coleta"].toString() == "true" || configuracoes["nu_forca_coleta"].toString() == "True"){
                return true;
            }
        }
    }
    return false;
}

bool CacicThread::eviarColetaDiff(){
    if(QFile::exists(ccacic->getCacicMainFolder() + "/coletaDiff.json")){
        bool ok = false;
        QJsonObject jsonColeta = this->ccacic->getJsonFromFile(this->applicationDirPath + "/coletaDiff.json");
        if (!jsonColeta.isEmpty()){
            QJsonObject retornoColeta;
            QLogger::QLog_Info(Identificadores::LOG_DAEMON_THREAD, QString("Enviando coleta Diff ao gerente."));
            retornoColeta = this->OCacicComm->comm(Identificadores::ROTA_COLETA_DIFF, &ok, jsonColeta , true);
            if(retornoColeta.contains("error")) {
                QLogger::QLog_Info(Identificadores::LOG_DAEMON_THREAD, QString("Falha ao enviar a coleta Diff: " + retornoColeta["error"].toString()));
            }
            return ok;
        } else {
            QLogger::QLog_Info(Identificadores::LOG_DAEMON_THREAD, QString("Falha ao ler a coleta Diff: Arquivo JSON vazio ou inexistente."));
            return false;
        }
    }
    return false;
}

bool CacicThread::realizarEnviodeColeta(){
    bool ok = false;
    QJsonObject jsonColeta = this->ccacic->getJsonFromFile(this->applicationDirPath + "/coleta.json");
    if (!jsonColeta.isEmpty()){
        QJsonObject retornoColeta;
        QLogger::QLog_Info(Identificadores::LOG_DAEMON_THREAD, QString("Enviando coleta ao gerente."));
        retornoColeta = this->OCacicComm->comm(Identificadores::ROTA_COLETA, &ok, jsonColeta , true);
        if (ok){
            if(!retornoColeta.isEmpty() && !retornoColeta.contains("error")){
                QVariantMap enviaColeta;
                enviaColeta["enviaColeta"] = false;
                ccacic->setValueToRegistry("Lightbase", "Cacic", enviaColeta);
            }
        } else if(retornoColeta.contains("error")) {
            QLogger::QLog_Info(Identificadores::LOG_DAEMON_THREAD, QString("Falha na coleta: " + retornoColeta["error"].toString()));
        }
        return ok;
    } else {
        QLogger::QLog_Info(Identificadores::LOG_DAEMON_THREAD, QString("Falha na coleta: Arquivo JSON vazio ou inexistente."));
        return false;
    }
}

bool CacicThread::enviarColeta() {
    if(this->nomeModulo == "gercols" && QFile::exists(ccacic->getCacicMainFolder() + "/coleta.json")){
        if(!verificaForcarColeta()){
            if (ccacic->getValueFromRegistry("Lightbase", "Cacic", "enviaColeta").toBool()){
                if(realizarEnviodeColeta()){
                    return true;
                }
                return false;
            }else{
                QLogger::QLog_Info(Identificadores::LOG_DAEMON_THREAD, QString("Sem diferença na coleta."));
                return false;
            }
        }else{
            if(realizarEnviodeColeta()){
                QLogger::QLog_Info(Identificadores::LOG_DAEMON_THREAD, QString("Enviou coleta forçadamente."));
                return true;
            }
            return false;
        }
    }
    return false;
}

void CacicThread::setCMutex(QMutex *value)
{
    cMutex = value;
}

void CacicThread::registraInicioColeta()
{
    QLogger::QLog_Info(Identificadores::LOG_DAEMON_THREAD,"Coleta iniciada em: " + QDateTime::currentDateTime().toLocalTime().toString());
}

void CacicThread::registraFimColeta(bool tipo)
{
    if(tipo){
        QLogger::QLog_Info(Identificadores::LOG_DAEMON_THREAD,"Coleta finalizada com SUCESSO em: " + QDateTime::currentDateTime().toLocalTime().toString());
    }else{
        QLogger::QLog_Info(Identificadores::LOG_DAEMON_THREAD,"Coleta finalizada com ERRO em: " + QDateTime::currentDateTime().toLocalTime().toString());
    }
}

void CacicThread::iniciarInstancias(){
    logManager = QLogger::QLoggerManager::getInstance();
    logManager->addDestination(this->applicationDirPath + "/Logs/cacic.log",Identificadores::LOG_DAEMON_THREAD,QLogger::InfoLevel);
    logManager->addDestination(this->applicationDirPath + "/Logs/cacic_error.log",Identificadores::LOG_DAEMON_THREAD,QLogger::ErrorLevel);
}
