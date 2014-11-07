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
            registraFimColeta("SUCESSO");
            if(enviarColeta()){
                QLogger::QLog_Info("Cacic Daemon (Thread)", QString("Coleta enviada com sucesso."));
            }else{
                QLogger::QLog_Info("Cacic Daemon (Thread)", QString("Erro ao enviar a coleta."));
            }
        }else{
            if((!proc.atEnd()) || (proc.exitStatus() == QProcess::CrashExit)){
                registraFimColeta("ERRO");
                proc.kill();
            }
        }
        proc.close();
    } else {
        QLogger::QLog_Error("Cacic Daemon (Thread)", QString("Módulo inexistente."));
    }
    cMutex->unlock();
    QLogger::QLog_Info("Cacic Daemon (Thread)", QString("Semáforo aberto com sucesso."));
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

bool CacicThread::enviarColeta()
{
    /*
     * fazer verificação se é preciso enviar a coleta;
     *
     */
    if(this->nomeModulo == "gercols" && QFile::exists(ccacic->getCacicMainFolder() + "/coleta.json")){
        //Envio do json gerado na coleta
        bool ok = false;
        if (ccacic->getValueFromRegistry("Lightbase", "Cacic", "enviaColeta").toBool()){
            QJsonObject jsonColeta = this->ccacic->getJsonFromFile(this->applicationDirPath + "/coleta.json");
            if (!jsonColeta.isEmpty()){
                QJsonObject retornoColeta;
                QLogger::QLog_Info("Cacic Daemon (Thread)", QString("Enviando coleta ao gerente."));
                retornoColeta = this->OCacicComm->comm("/ws/neo/coleta", &ok, jsonColeta , true);
                if (ok){
                    QVariantMap enviaColeta;
                    enviaColeta["enviaColeta"] = false;
                    ccacic->setValueToRegistry("Lightbase", "Cacic", enviaColeta);
                } else if(retornoColeta.contains("error")) {
                    QLogger::QLog_Info("Cacic Daemon (Thread)", QString("Falha na coleta: " + retornoColeta["error"].toString()));
                }
                return ok;
            } else
                QLogger::QLog_Info("Cacic Daemon (Thread)", QString("Falha na coleta: Arquivo JSON vazio ou inexistente."));
                return true;
        } else {
            QLogger::QLog_Info("Cacic Daemon (Thread)", QString("Sem diferença na coleta."));
            return true;
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
    QLogger::QLog_Info("Cacic Daemon (Thread)","Coleta iniciada em: " + QDateTime::currentDateTime().toLocalTime().toString());
}

void CacicThread::registraFimColeta(QString msg)
{
    QLogger::QLog_Info("Cacic Daemon (Thread)","Coleta finalizada com " + msg + " em: " + QDateTime::currentDateTime().toLocalTime().toString());
}

void CacicThread::iniciarInstancias(){
    logManager = QLogger::QLoggerManager::getInstance();
    logManager->addDestination(this->applicationDirPath + "/Logs/cacic.log","Cacic Daemon (Thread)",QLogger::InfoLevel);
    logManager->addDestination(this->applicationDirPath + "/Logs/cacic_error.log","Cacic Daemon (Thread)",QLogger::ErrorLevel);
}
