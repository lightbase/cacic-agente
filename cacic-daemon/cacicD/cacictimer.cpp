#include "cacictimer.h"

CacicTimer::CacicTimer()
{
    OCacicComm = new CacicComm();
    ccacic = new CCacic();
    timer = new QTimer(this);
    logManager = QLogger::QLoggerManager::getInstance();
    connect(timer,SIGNAL(timeout()),this,SLOT(mslot()));
}

void CacicTimer::iniciarTimer(int x, QString applicationDirPath)
{
    setApplicationDirPath(applicationDirPath);
    logManager->addDestination(getApplicationDirPath().append("/cacicLog.txt"), QStringList("Cacic Daemon"), QLogger::InfoLevel);
    logManager->addDestination(getApplicationDirPath().append("/cacicLog.txt"), QStringList("Cacic Daemon"), QLogger::ErrorLevel);
    timer->start(x);
}

void CacicTimer::mslot(){
    if(getTest()){
        //manager->QLogger::QLog_Trace("ModuleName", "Message: ");
        if(getConfig()){
            qDebug() << "getConfig() success. - " + QDateTime::currentDateTime().toLocalTime().toString();
            QLogger::QLog_Info("Cacic Daemon", "getConfig() executado com sucesso.");

            // Compara o novo arquivo de configuração com um antigo e se forem diferentes
            // mantem o mais recente; caso iguais simplesmente apaga o novo.
            QFile *fileConfig = new QFile(getApplicationDirPath().append("/getConfig.json"));
            QFile *fileConfigNew = new QFile(getApplicationDirPath().append("/getConfigNew.json"));

            if( fileConfig->exists() && fileConfigNew->exists() ){
                if( Md5IsEqual(QVariant::fromValue(fileConfig), QVariant::fromValue(fileConfigNew)) ) {
                    fileConfigNew->remove();
                    delete fileConfigNew;
                } else {
                    // Renomeia getConfigNew.json para getConfig.json
                    fileConfig->remove();
                    delete fileConfig;
                    fileConfigNew->rename("getConfigNew.json","getConfig.json");
                    delete fileConfigNew;
                }
                jsonConfig = ccacic->getJsonFromFile(getApplicationDirPath().append("/getConfig.json"));
            } else if( fileConfig->exists() ){
                jsonConfig = ccacic->getJsonFromFile(getApplicationDirPath().append("/getConfig.json"));
            } else {
                QLogger::QLog_Error("Cacic Daemon", "Arquivo de configuração não criado.");
            }

            /* lê json de configurações e armazena quais módulos executáveis.
             * E faz o mesmo tipo de comparação de hashs, com o fim de:
             * ou mantem o binário do módulo ou baixa um novo.
             */
            foreach( QVariant vectorsContent, jsonConfig["modulos"].toArray().toVariantList().toVector() )
                moduleList.append( vectorsContent.toString() );

            foreach( QString modulo, moduleList ) {

            }

            //aqui irá abrir o semaforo e iniciar a coleta.

        }else{
            qDebug() << "getConfig() failed. - " + QDateTime::currentDateTime().toLocalTime().toString();
            QLogger::QLog_Error("Cacic Daemon", "Falha na obtenção do arquivo de configuração.");
        }
    }else{
        qDebug() << "getTest() failed. - " + QDateTime::currentDateTime().toLocalTime().toString();
        QLogger::QLog_Error("Cacic Daemon", "Falha na execução do getTest().");
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
            ccacic->setJsonToFile(jsonresult, getApplicationDirPath().append("/getConfigNew.json"));
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

bool CacicTimer::Md5IsEqual(QVariant document01,QVariant document02){
    QString getconfigMD5 = QString(QCryptographicHash::hash(
                                       (document01.toByteArray()),QCryptographicHash::Md5).toHex());
    QString getconfigMD52 = QString(QCryptographicHash::hash(
                                        (document02.toByteArray()),QCryptographicHash::Md5).toHex());
    if(getconfigMD5 == getconfigMD52){
        return true;
    }else{
        return false;
    }
}
