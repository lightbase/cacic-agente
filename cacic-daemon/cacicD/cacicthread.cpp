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
    }else{
        if((!proc.atEnd()) || (proc.exitStatus() == QProcess::CrashExit)){
            registraFimColeta("ERRO");
            proc.kill();
        }
    }
    cMutex->unlock();
    QLogger::QLog_Info("Cacic Daemon (Thread)", QString("Semáforo aberto com sucesso."));
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
    logManager->addDestination(this->applicationDirPath + "/Logs/cacicLog.txt","Cacic Daemon (Thread)",QLogger::InfoLevel);
    logManager->addDestination(this->applicationDirPath + "/Logs/cacicLog.txt","Cacic Daemon (Thread)",QLogger::ErrorLevel);
}
