#include "cacicthread.h"

CacicThread::CacicThread(QString applicationDirPath)
{
    this->applicationDirPath = applicationDirPath;
    logcacic = new LogCacic(LOG_DAEMON_THREAD, applicationDirPath+"/Logs");
    this->timeout = 0;
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
        if (this->timeout > 0)
            QTimer::singleShot(this->timeout, &proc, SLOT(kill()));
        proc.execute(this->moduloDirPath);
        if((proc.atEnd()) && (proc.exitStatus() == QProcess::NormalExit)){
            registraExecucao(true, proc.errorString());
        }else{
            if(!proc.atEnd()){
                registraExecucao(false, proc.errorString());
                proc.kill();
            }
        }
        proc.close();
    } else {
        logcacic->escrever(LogCacic::InfoLevel, QString("Módulo inexistente."));
        logcacic->escrever(LogCacic::ErrorLevel, QString("Módulo inexistente."));
    }
    cMutex->unlock();
}
int CacicThread::getTimeout() const
{
    return timeout;
}

void CacicThread::setTimeout(int value)
{
    timeout = value;
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
