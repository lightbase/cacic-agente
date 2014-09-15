#include "cacictimer.h"

CacicTimer::CacicTimer(QString dirpath)
{
    definirDirGercols(dirpath);
    setApplicationDirPath(dirpath);
    iniciarInstancias();
    connect(timer,SIGNAL(timeout()),this,SLOT(mslot()));
}

CacicTimer::~CacicTimer()
{
    logManager->closeLogger();
    delete logManager;
    delete ccacic;
    delete OCacicComm;
}

void CacicTimer::iniciarTimer(int x)
{
    timer->start(x);
}

void CacicTimer::mslot(){
    QLogger::QLog_Info("Cacic Daemon (Timer)", QString("mslot();"));
    cMutex->lock();
    QLogger::QLog_Info("Cacic Daemon (Timer)", QString("Semáforo fechado."));
    //if(getTest()){
    //    QLogger::QLog_Info("Cacic Daemon (Timer)", QString("getTeste() success."));
    //    if(getConfig()){
    //        QLogger::QLog_Info("Cacic Daemon (Timer)", QString("getConfig() success."));
    iniciarGercols();
    //    }
    // }
    cMutex->unlock();
    QLogger::QLog_Info("Cacic Daemon (Timer)", QString("Semáforo aberto."));
}

//            QLogger::QLog_Info("Cacic Timer", QString("semáforo aberto."));
//            qDebug() << "getConfig() success. - " + QDateTime::currentDateTime().toLocalTime().toString();
//            QLogger::QLog_Info("Cacic Daemon (Timer)", "getConfig() executado com sucesso.");

//            // Compara o novo arquivo de configuração com um antigo e se forem diferentes
//            // mantem o mais recente; caso iguais simplesmente apaga o novo.
//            QFile *fileOld;
//            QFile *fileNew;

//            fileOld = new QFile(getApplicationDirPath().append("/getConfig.json"));
//            fileNew = new QFile(getApplicationDirPath().append("/getConfigNew.json"));
//            if( fileOld->exists() && fileNew->exists() ){
//                if( Md5IsEqual(QVariant::fromValue(fileOld), QVariant::fromValue(fileNew)) ) {
//                    fileNew->remove();
//                } else {
//                    // Renomeia getConfigNew.json para getConfig.json
//                    fileOld->remove();
//                    fileNew->rename("getConfigNew.json","getConfig.json");
//                }
//                jsonConfig = ccacic->getJsonFromFile(getApplicationDirPath().append("/getConfig.json"));
//            } else if( fileOld->exists() ){
//                jsonConfig = ccacic->getJsonFromFile(getApplicationDirPath().append("/getConfig.json"));
//            } else {
//                QLogger::QLog_Error("Cacic Daemon (Timer)", "Arquivo de configuração não criado.");
//            }
//            delete fileOld;
//            delete fileNew;

//            /* lê json de configurações e armazena quais módulos executáveis.
//             * E faz o mesmo tipo de comparação de hashs, com o fim de:
//             * ou mantem o binário do módulo ou baixa um novo.
//             */

//            /* Aqui estou assumindo um formato do .json em que:
//             * há a key modulos contem uma lista com o nome dos executaveis e os seus valores hash md5
//             * há a key metodo que explicita o método de download dos executaveis
//             */
//            foreach( QJsonValue moduloValues, jsonConfig["modulos"].toArray() ) {
//                QString moduloKey, moduloValue;

//                moduloKey = moduloValues.toObject()["md5"].toString();
//                moduloValue = moduloValues.toObject()["nome"].toString();

//                moduleMap.insert(moduloKey, moduloValue);
//            }
//            metodoDownload = jsonConfig["metodo"].toString();

//            int countExecNotFound = 0;
//            QMap<QString, QString>::const_iterator mapIterator = moduleMap.constBegin();
//            while (mapIterator != moduleMap.constEnd()) {
//                QString nomeModulo = mapIterator.value();
//                QString hashModulo = mapIterator.key();

//                // Calcula hash do binario atual
//#if defined(Q_OS_WIN)
//                fileOld = new QFile(getApplicationDirPath().append("/").append(modulo).append(".exe"));
//#else
//                fileOld = new QFile(getApplicationDirPath().append("/").append(nomeModulo));
//#endif
//                if(!fileOld->exists()) {
//                    QLogger::QLog_Error("Cacic Daemon (Timer)", QString("Módulo ").append(nomeModulo).append(" não encontrado."));

//                    countExecNotFound++;
//                    if( countExecNotFound == moduleMap.size() ) {
//                        QLogger::QLog_Error("Cacic Daemon (Timer)", "Não foi possível encontrar nenhum módulo executável!");
//                        return;
//                    }
//                    continue;
//                }

//                QString oldMd5 = QString(QCryptographicHash::hash(fileOld->readAll(),QCryptographicHash::Md5).toHex());

//                if ( oldMd5 != hashModulo ) {

//                    fileOld->rename(getApplicationDirPath().append("/").append(nomeModulo),
//                                    getApplicationDirPath().append("/").append("nomeModulo").append("Old") );

//                    // Download nova versão do executável
//                    if(metodoDownload == "ftp" || metodoDownload == "") {
//                        //                        OCacicComm->ftpDownload( QString("/ws/get/").append(nomeModulo) );
//                    }


//                    fileOld->remove();
//                    delete fileOld;
//                }

//                mapIterator++;
//            }
//        }else{
//            qDebug() << "getConfig() failed. - " + QDateTime::currentDateTime().toLocalTime().toString();
//            QLogger::QLog_Error("Cacic Daemon (Timer)", "Falha na obtenção do arquivo de configuração.");
//        }
//    }else{
//        qDebug() << "getTest() failed. - " + QDateTime::currentDateTime().toLocalTime().toString();
//        QLogger::QLog_Error("Cacic Daemon (Timer)", "Falha na execução do getTest().");
//    }
//        }
//    }
//}

bool CacicTimer::getTest(){
    try{
        bool ok;
        QJsonObject as;
        as["computador"] = OCacic_Computer.toJsonObject();
        QJsonObject jsonresult = OCacicComm->comm("/ws/neo/login", &ok, as);
        //        if(jsonresult.contains("error")){
        //            return false;
        //        }
        try{
            ccacic->setJsonToFile(jsonresult, this->applicationDirPath + "/getTest.json");
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
        bool ok;
        QJsonObject as;
        as["computador"] = OCacic_Computer.toJsonObject();
        QJsonObject jsonresult = OCacicComm->comm("/ws/neo/login", &ok, as);
        //        if(jsonresult.contains("error")){
        //            return false;
        //        }
        try{
            ccacic->setJsonToFile(jsonresult, this->applicationDirPath + "/getConfigNew.json");
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
    QLogger::QLog_Info("Cacic Daemon (Timer)","Coleta iniciada em: " + QDateTime::currentDateTime().toLocalTime().toString());
}

QString CacicTimer::getDirProgram() const
{
    return dirProgram;
}

void CacicTimer::setDirProgram(const QString &value)
{
    dirProgram = value;
}


void CacicTimer::iniciarGercols()
{
    registraInicio();
    QDir::setCurrent(this->applicationDirPath);
    QProcess proc;
    proc.setWorkingDirectory(this->applicationDirPath);
    proc.execute(getDirProgram());
    if((proc.atEnd()) && (proc.exitStatus() == QProcess::NormalExit)){
        registraFim("SUCESSO");
    }else{
        proc.waitForFinished(120000);
        if((!proc.atEnd()) || (proc.exitStatus() == QProcess::CrashExit)){
            registraFim("ERRO");
            proc.kill();
        }
    }
}

void CacicTimer::setApplicationDirPath(const QString &value)
{
    this->applicationDirPath = value;
}


void CacicTimer::registraFim(QString msg)
{
    QLogger::QLog_Info("Cacic Daemon (Timer)","Coleta finalizada com " + msg + " em: " + QDateTime::currentDateTime().toLocalTime().toString());
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

void CacicTimer::iniciarInstancias(){
    logManager = QLogger::QLoggerManager::getInstance();
    logManager->addDestination(this->applicationDirPath + "/cacicLog.txt","Cacic Daemon (Timer)",QLogger::InfoLevel);
    logManager->addDestination(this->applicationDirPath + "/cacicLog.txt","Cacic Daemon (Timer)",QLogger::ErrorLevel);
    ccacic = new CCacic();
    timer = new QTimer(this);
    cMutex = new QMutex(QMutex::Recursive);
    OCacicComm = new CacicComm();
    OCacicComm->setUrlSsl("https://10.1.0.137/cacic/web/app_dev.php");
    OCacicComm->setUsuario("cacic");
    OCacicComm->setPassword("cacic123");
}

void CacicTimer::definirDirGercols(QString appDirPath){
#if defined (Q_OS_WIN)
    setDirProgram(appDirPath + "\cacic-gercols.exe");
#elif defined (Q_OS_LINUX)
    setDirProgram(appDirPath + "/cacic-gercols");
#endif
}
