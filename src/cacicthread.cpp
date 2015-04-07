#include "cacicthread.h"

CacicThread::CacicThread(QString applicationDirPath)
{
    this->applicationDirPath = applicationDirPath;
    logcacic = new LogCacic(LOG_DAEMON_THREAD, applicationDirPath+"/Logs");
    this->timeoutSec = 0;
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
    registraInicioExecucao();
    if (QFile::exists(this->moduloDirPath)){
        QDir::setCurrent(this->applicationDirPath);
        QProcess proc;
        proc.setWorkingDirectory(this->applicationDirPath);
        if (this->timeoutSec > 0)
            QTimer::singleShot(this->timeoutSec, &proc, SLOT(kill()));
        proc.execute(this->moduloDirPath);
        if((proc.atEnd()) && (proc.exitStatus() == QProcess::NormalExit)){
            registraExecucao(true, proc.errorString());
        }else{
            if(!proc.atEnd()){
                registraExecucao(false, proc.errorString());
                proc.kill();
            }
        }
        //parece que signals são somente privados. Confirmar quando voltar.
        setLastStatus(proc.exitStatus());
        setLastError(proc.errorString());
        proc.close();
    } else {
        logcacic->escrever(LogCacic::InfoLevel, QString("Módulo inexistente."));
        logcacic->escrever(LogCacic::ErrorLevel, QString("Módulo inexistente."));
    }
    emit endExecution();
    cMutex->unlock();
}
QProcess::ExitStatus CacicThread::getLastStatus() const
{
    return lastStatus;
}

void CacicThread::setLastStatus(const QProcess::ExitStatus &value)
{
    lastStatus = value;
}
QString CacicThread::getLastError() const
{
    return lastError;
}

void CacicThread::setLastError(const QString &value)
{
    lastError = value;
}



int CacicThread::getTimeoutSec() const
{
    return timeoutSec;
}

void CacicThread::setTimeoutSec(int value)
{
    timeoutSec = value;
}


void CacicThread::setNomeModulo(const QString &value)
{
    nomeModulo = value;
}

void CacicThread::setCMutex(QMutex *value)
{
    cMutex = value;
}

void CacicThread::registraInicioExecucao()
{
    logcacic->escrever(LogCacic::InfoLevel, "Thread iniciada em: " + QDateTime::currentDateTime().toLocalTime().toString());
}

void CacicThread::registraExecucao(bool tipo, QString status)
{
    if(tipo){
        logcacic->escrever(LogCacic::InfoLevel, "Thread finalizada com SUCESSO");
    }else{
        logcacic->escrever(LogCacic::InfoLevel, "Thread finalizada com falha.");
        logcacic->escrever(LogCacic::ErrorLevel, "Thread finalizada com erro: " + status);
    }
}
