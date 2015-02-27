#include "cacicthread.h"

CacicThread::CacicThread(QString applicationDirPath)
{
    this->applicationDirPath = applicationDirPath;
    logcacic = new LogCacic(Identificadores::LOG_DAEMON_THREAD, applicationDirPath+"/Logs");
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
        logcacic->escrever(LogCacic::ErrorLevel, QString("Módulo inexistente."));
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
    logcacic->escrever(LogCacic::InfoLevel, "Thread iniciada em: " + QDateTime::currentDateTime().toLocalTime().toString());
}

void CacicThread::registraFimColeta(bool tipo)
{
    if(tipo){
        logcacic->escrever(LogCacic::InfoLevel, "Thread finalizada com SUCESSO em: " + QDateTime::currentDateTime().toLocalTime().toString());
    }else{
        logcacic->escrever(LogCacic::InfoLevel, "Thread finalizada com ERRO em: " + QDateTime::currentDateTime().toLocalTime().toString());
    }
}
