#include "cacictimer.h"

CacicTimer::CacicTimer()
{
    OCacicComm = new CacicComm();
    ccacic = new CCacic();
    timer = new QTimer(this);
    //manager = QLogger::QLoggerManager::getInstance();
    //manager->addDestination("cacicLog.txt", QStringList("CacicD"), QLogger::LogLevel);
    connect(timer,SIGNAL(timeout()),this,SLOT(mslot()));
}

void CacicTimer::iniciarTimer(int x, QString applicationDirPath)
{
    setApplicationDirPath(applicationDirPath);
    timer->start(x);
}

void CacicTimer::mslot(){
    if(getTest()){
        //manager->QLogger::QLog_Trace("ModuleName", "Message: ");
        if(getConfig()){
            qDebug() << "getConfig() success. - " + QDateTime::currentDateTime().toLocalTime().toString();

            //aqui irá abrir o semaforo e iniciar a coleta.

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
        if(jsonresult.contains("error")){
            return false;
        }
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
        if(jsonresult.contains("error")){
            return false;
        }
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

QString CacicTimer::getApplicationDirPath() const
{
    return applicationDirPath;
}

void CacicTimer::setApplicationDirPath(const QString &value)
{
    applicationDirPath = value;
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
