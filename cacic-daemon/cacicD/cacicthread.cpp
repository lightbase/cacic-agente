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
}

void CacicThread::setCcacic(CCacic *value)
{
    ccacic = value;
}

void CacicThread::setNomeModulo(const QString &value)
{
    nomeModulo = value;
}

void CacicThread::setCMutex(QMutex *value)
{
    cMutex = value;
}

void CacicThread::registraInicioColeta()
{
    QLogger::QLog_Info(Identificadores::LOG_DAEMON_THREAD,"Thread iniciada em: " + QDateTime::currentDateTime().toLocalTime().toString());
}

void CacicThread::registraFimColeta(bool tipo)
{
    if(tipo){
        QLogger::QLog_Info(Identificadores::LOG_DAEMON_THREAD,"Thread finalizada com SUCESSO em: " + QDateTime::currentDateTime().toLocalTime().toString());
    }else{
        QLogger::QLog_Info(Identificadores::LOG_DAEMON_THREAD,"Thread finalizada com ERRO em: " + QDateTime::currentDateTime().toLocalTime().toString());
    }
}

void CacicThread::iniciarInstancias(){
    logManager = QLogger::QLoggerManager::getInstance();
    logManager->addDestination(this->applicationDirPath + "/Logs/cacic.log",Identificadores::LOG_DAEMON_THREAD,QLogger::InfoLevel);
    logManager->addDestination(this->applicationDirPath + "/Logs/cacic_error.log",Identificadores::LOG_DAEMON_THREAD,QLogger::ErrorLevel);
}
