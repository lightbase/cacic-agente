#include "cacictimer.h"

CacicTimer::CacicTimer(QString dirpath)
{
    qDebug() << "iniciou o constructor";
    definirDirGercols(dirpath);
    setApplicationDirPath(dirpath);
    iniciarInstancias();
    connect(timer,SIGNAL(timeout()),this,SLOT(mslot()));
}

void CacicTimer::iniciarTimer(int x)
{
    timer->start(x);
}

void CacicTimer::mslot(){
    cMutex->lock();
    if(getTest()){
        if(getConfig()){
            QLogger::QLog_Info("Cacic Timer", QString("semáforo fechado."));
            iniciarGercols();
            cMutex->unlock();
            QLogger::QLog_Info("Cacic Timer", QString("semáforo aberto."));
        }else{
            qDebug() << "getConfig() failed. - " + QDateTime::currentDateTime().toLocalTime().toString();
        }
    }else{
        qDebug() << "getTest() failed. - " + QDateTime::currentDateTime().toLocalTime().toString();
    }
}

bool CacicTimer::getTest(){
    try{
        OCacicComm->setUrlGerente("http://10.1.0.137/cacic/web/app_dev.php");
        OCacicComm->setUsuario("cacic");
        OCacicComm->setPassword("cacic123");
        bool ok;
        QJsonObject as;
        as["computador"] = OCacic_Computer.toJsonObject();
        QJsonObject jsonresult = OCacicComm->comm("/ws/neo/login", &ok, as);
        //        if(jsonresult.contains("error")){
        //            return false;
        //        }
        try{
            ccacic->setJsonToFile(jsonresult, getApplicationDirPath() + "/getTest.json");
            return true;
        } catch (...) {
            qDebug() << "Erro ao salvar o arquivo de configurações.";
            return false;
        }
    } catch (...){
        qDebug() << "Erro ao conectar com o servidor.";
        return false;
    }
}

bool CacicTimer::getConfig(){
    try{
        OCacicComm->setUrlGerente("http://10.1.0.137/cacic/web/app_dev.php");
        OCacicComm->setUsuario("cacic");
        OCacicComm->setPassword("cacic123");
        bool ok;
        QJsonObject as;
        as["computador"] = OCacic_Computer.toJsonObject();
        QJsonObject jsonresult = OCacicComm->comm("/ws/neo/login", &ok, as);
        //        if(jsonresult.contains("error")){
        //            return false;
        //        }
        try{
            ccacic->setJsonToFile(jsonresult, getApplicationDirPath() + "/getConfig.json");
            return true;
        } catch (...) {
            qDebug() << "Erro ao salvar o arquivo de configurações.";
            return false;
        }
    } catch (...){
        qDebug() << "Erro ao conectar com o servidor.";
        return false;
    }
}

void CacicTimer::registraInicio()
{
}

QString CacicTimer::getDirProgram() const
{
    return dirProgram;
}

QString CacicTimer::getApplicationDirPath() const
{
    return applicationDirPath;
}

void CacicTimer::setDirProgram(const QString &value)
{
    dirProgram = value;
}


void CacicTimer::iniciarGercols()
{

    QProcess proc;
    proc.children();
    proc.execute(getDirProgram());
    if(proc.atEnd()){

    }
    //    QString retorno = ccacic->startProcess(getDirProgram(),true,ok);
    //    qDebug() << retorno;
    QLogger::QLog_Info("Cacic Timer","processo finalizado.");
    //    if (myProcess.waitForFinished(-1)){
    //        qDebug() << "erro:" << myProcess.errorString();
    //    }else{
    //        qDebug() << "não deu erro:" << myProcess.readAll();
    //        QLogger::QLog_Info("Cacic Timer","processo está aberto.");
    //    }
}

void CacicTimer::setApplicationDirPath(const QString &value)
{
    this->applicationDirPath = value;
}


void CacicTimer::registraFim()
{
}

bool CacicTimer::compararHashMD5(QJsonDocument getconfigfile,QJsonDocument getConfig){
    QString getconfigMD5 = QString(QCryptographicHash::hash(
                                       (getconfigfile.toVariant().toByteArray()),QCryptographicHash::Md5).toHex());
    QString getconfigMD52 = QString(QCryptographicHash::hash(
                                        (getConfig.toVariant().toByteArray()),QCryptographicHash::Md5).toHex());
    if(getconfigMD5 == getconfigMD52){
        return true;
    }else{
        return false;
    }
}

void CacicTimer::iniciarInstancias(){
    logManager = QLogger::QLoggerManager::getInstance();
    logManager->addDestination(getApplicationDirPath().append("/log.txt"),"Cacic Timer",QLogger::InfoLevel);
    logManager->addDestination(getApplicationDirPath().append("/log.txt"),"Cacic Timer",QLogger::ErrorLevel);
    OCacicComm = new CacicComm();
    ccacic = new CCacic();
    timer = new QTimer(this);
    cMutex = new QMutex(QMutex::Recursive);
    QLogger::QLog_Info("Cacic Timer", QString("terminou as instancias"));
}

void CacicTimer::definirDirGercols(QString applicationDirPath){
#if defined (Q_OS_WIN)
    setDirProgram(applicationDirPath.append("\cacic-gercols.exe"));
#elif defined (Q_OS_LINUX)
    setDirProgram(applicationDirPath.append("/cacic-gercols"));
#endif
}
