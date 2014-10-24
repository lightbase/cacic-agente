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
    if(this->nomeModulo == "gercols" && QFile::exists("coleta.json")){
        //Envio do json gerado na coleta
        bool ok = false;
        QJsonObject jsonColeta = this->ccacic->getJsonFromFile(this->applicationDirPath + "/coleta.json");
        this->OCacicComm->comm("/ws/neo/coleta", &ok, jsonColeta , false);
        return &ok;
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
    logManager->addDestination(this->applicationDirPath + "/Logs/cacicLog.log","Cacic Daemon (Thread)",QLogger::InfoLevel);
    logManager->addDestination(this->applicationDirPath + "/Logs/cacicLog.log","Cacic Daemon (Thread)",QLogger::ErrorLevel);
}
