#include "cacictimer.h"

CacicTimer::CacicTimer(QString dirpath)
{
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

void CacicTimer::reiniciarTimer(){
    timer->stop();
    timer->start(getPeriodicidadeExecucao());
}


void CacicTimer::iniciarTimer()
{
    timer->start(getPeriodicidadeExecucao());
}

void CacicTimer::mslot(){
    try{
        verificarPeriodicidadeJson();
        bool ok;
        QJsonObject resposta = OCacicComm->login(&ok);
        if(resposta.isEmpty() || resposta.contains("error")){
            //de vez enquando a conexão da erro, é bom tentar 2 vezes pra garantir.
            QLogger::QLog_Info("Cacic Daemon (Timer)", "Erro no primeiro login.");
            resposta = OCacicComm->login(&ok);
            if(resposta.isEmpty() || resposta.contains("error")){
                QLogger::QLog_Info("Cacic Daemon (Timer)", "Erro no segundo login.");
                return;
            }else{
                QLogger::QLog_Info("Cacic Daemon (Timer)", "getLogin() success.");
            }
        }else{
            QLogger::QLog_Info("Cacic Daemon (Timer)", "getLogin() success.");
        }
    }catch (...){
        QLogger::QLog_Info("Cacic Daemon (Timer)", QString("Não foi possivel verificar a periodicidade no getConfig.json"));
    }

    verificarEIniciarQMutex();

    if(getTest()){
        QLogger::QLog_Info("Cacic Daemon (Timer)", QString("getTeste() success."));
        if(getConfig()){
            QLogger::QLog_Info("Cacic Daemon (Timer)", QString("getConfig() success."));
            //            QStringList nomesModulos = verificarModulos();
            //            if ( !nomesModulos.empty() ) {
            //             foreach( QString nome, nomesModulos ) {
            QString nome = "gercols";
            definirDirModulo(getApplicationDirPath(), nome);
            cacicthread->setCcacic(ccacic);
            cacicthread->setOCacicComm(OCacicComm);
            cacicthread->setNomeModulo(nome);
            cacicthread->setCMutex(cMutex);
            cacicthread->setModuloDirPath(getDirProgram());
            cacicthread->start(QThread::NormalPriority);
//            if(nome == "gercols" ){
//                //Envio do json gerado na coleta
//                bool ok = false;
//                QJsonObject jsonColeta = ccacic->getJsonFromFile(this->applicationDirPath + "/coleta.json");
//                OCacicComm->comm("/ws/neo/coleta", &ok, jsonColeta , false);
//                if(&ok){
//                    QLogger::QLog_Info("Cacic Daemon (Timer)", QString("coleta enviada com sucesso."));
//                }
//            }
        }else{
            QLogger::QLog_Error("Cacic Daemon (Timer)", "Falha na obtenção do arquivo de configuração.");
        }
    }else{
        QLogger::QLog_Error("Cacic Daemon (Timer)", "Falha na execução do getTest().");
    }
}


void CacicTimer::verificarEIniciarQMutex(){
    if(!cacicthread->isRunning()){
        cMutex->lock();
        QLogger::QLog_Info("Cacic Daemon (Timer)", "Semáforo fechado com sucesso.");
    }else{
        QLogger::QLog_Info("Cacic Daemon (Timer)", "Possivelmente o gercols travou e será finalizado.");
        try{
            cacicthread->terminate();
            QLogger::QLog_Info("Cacic Daemon (Timer)", "Gercols finalizado com sucesso.");
        }catch (...){
            QLogger::QLog_Error("Cacic Daemon (Timer)", "Falha ao finalizar gercols.");
            return;
        }
        cMutex->lock();
        QLogger::QLog_Info("Cacic Daemon (Timer)", "Semáforo fechado com sucesso.");
    }
}

QString CacicTimer::getApplicationDirPath() {
    return applicationDirPath;
}

bool CacicTimer::getTest(){
    try{
        bool ok;
        QJsonObject as;
        as["computador"] = OCacic_Computer.toJsonObject();
        QJsonObject jsonresult = OCacicComm->comm("/ws/neo/getTest", &ok, as, false);
        if(!ok){
            jsonresult = OCacicComm->comm("/ws/neo/getTest", &ok, as, false); // mais uma vez pra garantir.
        }
        if(jsonresult.contains("error")){
            return false;
        }
        try{
            ccacic->setJsonToFile(jsonresult.contains("reply") ? jsonresult["reply"].toObject() : jsonresult,
                                  this->applicationDirPath + "/getTest.json");
            return true; //acho que seria melhor retornar a variável 'ok'. Se der erro na conexão eu acho que não cai no catch.
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
        QJsonObject jsonresult = OCacicComm->comm("/ws/neo/config", &ok, as, false);
        if(!ok){
            jsonresult = OCacicComm->comm("/ws/neo/config", &ok, as, false); // mais uma vez pra garantir.
        }
        if(jsonresult.contains("error")){
            return false;
        }
        try{
            ccacic->setJsonToFile(jsonresult.contains("reply") ? jsonresult["reply"].toObject() : jsonresult,
                                  this->applicationDirPath + "/getConfigNew.json");
            return true; //mesma observação do getTest
        } catch (...) {
            qDebug() << "Erro ao salvar o arquivo de configurações.";
            return false;
        }
    } catch (...){
        qDebug() << "Erro ao conectar com o servidor.";
        return false;
    }
}

void CacicTimer::lerArquivoConfig ( const QJsonObject& jsonConfig )
{
    /* lê json de configurações e armazena quais módulos executáveis.
     * E faz o mesmo tipo de comparação de hashs, com o fim de:
     * ou mantem o binário do módulo ou baixa um novo.
     */
    foreach( QJsonValue individualModule, jsonConfig["modulos"].toArray() ) {
        QString moduloKey, moduloValue;

        moduloKey = individualModule.toObject()["hash"].toString();
        moduloValue = individualModule.toObject()["nome"].toString();

        moduleMap.insert(moduloKey, moduloValue);
    }

    if ( jsonConfig["metodoDownload"].isArray() ) {

        foreach (QJsonValue individualMetodo, jsonConfig["metodoDownload"].toArray() ) {
            QMap<QString, QString> newEntry;

            newEntry.insert(QString("tipo"), individualMetodo.toObject()["tipo"].toString() );
            newEntry.insert(QString("url"), individualMetodo.toObject()["url"].toString() );
            newEntry.insert(QString("path"), individualMetodo.toObject()["path"].toString() );
            newEntry.insert(QString("usuario"), individualMetodo.toObject()["usario"].toString() );
            newEntry.insert(QString("senha"), individualMetodo.toObject()["senha"].toString() );

            metodosDownload.append( newEntry );
        }
    } else {
        QMap<QString, QString> newEntry;

        newEntry.insert(QString("tipo"), jsonConfig["metodoDownload"].toObject()["tipo"].toString() );
        newEntry.insert(QString("url"), jsonConfig["metodoDownload"].toObject()["url"].toString() );
        newEntry.insert(QString("path"), jsonConfig["metodoDownload"].toObject()["path"].toString() );
        newEntry.insert(QString("usuario"), jsonConfig["metodoDownload"].toObject()["usario"].toString() );
        newEntry.insert(QString("senha"), jsonConfig["metodoDownload"].toObject()["senha"].toString() );

        metodosDownload.append( newEntry );
    }
}

QString CacicTimer::getDirProgram() const
{
    return dirProgram;
}

void CacicTimer::setDirProgram(const QString &value)
{
    dirProgram = value;
}


void CacicTimer::setApplicationDirPath(const QString &value)
{
    this->applicationDirPath = value;
}


bool CacicTimer::Md5IsEqual(QVariant document01,QVariant document02){
    QString getconfigMD5 = QString(QCryptographicHash::hash(
                                       (document01.toByteArray()),QCryptographicHash::Md5).toHex());
    QString getconfigMD52 = QString(QCryptographicHash::hash(
                                        (document02.toByteArray()),QCryptographicHash::Md5).toHex());
    return getconfigMD5 == getconfigMD52;
}

void CacicTimer::iniciarInstancias(){
    logManager = QLogger::QLoggerManager::getInstance();
    logManager->addDestination(this->applicationDirPath + "/Logs/cacicLog.txt","Cacic Daemon (Timer)",QLogger::InfoLevel);
    logManager->addDestination(this->applicationDirPath + "/Logs/cacicLog.txt","Cacic Daemon (Timer)",QLogger::ErrorLevel);
    ccacic = new CCacic();
    timer = new QTimer(this);
    cMutex = new QMutex(QMutex::Recursive);
    cacicthread = new CacicThread(this->applicationDirPath);
    OCacicComm = new CacicComm();
    OCacicComm->setUrlSsl("https://teste.cacic.cc");
    OCacicComm->setUrlGerente("teste.cacic.cc");
    OCacicComm->setUsuario("cacic");
    OCacicComm->setPassword("cacic123");
}

void CacicTimer::verificarPeriodicidadeJson()
{
    QJsonObject result = ccacic->getJsonFromFile(this->applicationDirPath + "/getConfig.json");
    if(!result.contains("error") && !result.isEmpty()){
        QJsonObject agenteConfigJson = result["agentcomputer"].toObject();
        QJsonObject configuracoes = agenteConfigJson["configuracoes"].toObject();
        if(getPeriodicidadeExecucao() != configuracoes["nu_intervalo_exec"].toString().toInt()){
            setPeriodicidadeExecucao(configuracoes["nu_intervalo_exec"].toString().toInt() * 60000);
            reiniciarTimer();
        }
    }else{
        QLogger::QLog_Error("Cacic Daemon (Timer)", QString("getConfig.json com erro ou vazio"));
    }
}


void CacicTimer::definirDirModulo(QString appDirPath, QString nome){
#if defined (Q_OS_WIN)
    setDirProgram(appDirPath + "\\" + nome + ".exe");
#elif defined (Q_OS_LINUX)
    setDirProgram(appDirPath + "/"+ nome);
#endif
}

int CacicTimer::getPeriodicidadeExecucao() const
{
    return periodicidadeExecucao;
}

void CacicTimer::setPeriodicidadeExecucao(int value)
{
    periodicidadeExecucao = value;
}


QStringList CacicTimer::verificarModulos(){
    // Compara o novo arquivo de configuração com um antigo e se forem diferentes
    // mantem o mais recente; caso iguais simplesmente apaga o novo.
    QFile *fileOld;
    QFile *fileNew;

    fileOld = new QFile(this->applicationDirPath + "/getConfig.json");
    fileNew = new QFile(this->applicationDirPath + "/getConfigNew.json");

    if( fileOld->exists() && fileNew->exists() ){
        if( Md5IsEqual(QVariant::fromValue(fileOld), QVariant::fromValue(fileNew)) ) {
            fileNew->remove();
        } else {
            // Renomeia getConfigNew.json para getConfig.json
            fileOld->remove();
            fileNew->rename("getConfigNew.json","getConfig.json");
        }
        jsonConfig = ccacic->getJsonFromFile(this->applicationDirPath + "/getConfig.json");
    } else if( fileOld->exists() ){
        jsonConfig = ccacic->getJsonFromFile(this->applicationDirPath + "/getConfig.json");
    } else {
        QLogger::QLog_Error("Cacic Daemon (Timer)", "Arquivo de configuração não criado.");
    }
    delete fileOld;
    delete fileNew;
    lerArquivoConfig(jsonConfig["agentcomputer"].toObject());

    QStringList nomesModulos;

    int countExecNotFound = 0;
    QMap<QString, QString>::const_iterator moduloIterator = moduleMap.constBegin();
    while (moduloIterator != moduleMap.constEnd()) {
        QString nomeModulo = moduloIterator.value();
        QString hashModulo = moduloIterator.key();
        // Calcula hash do binario atual
#if defined(Q_OS_WIN)
        fileOld = new QFile(this->applicationDirPath + "/" + nomeModulo + ".exe");
#else
        fileOld = new QFile(this->applicationDirPath + "/" + nomeModulo);
#endif
        if(!fileOld->exists()) {
            QLogger::QLog_Error("Cacic Daemon (Timer)", QString("Módulo ").append(nomeModulo).append(" não encontrado."));
            countExecNotFound++;

            if( countExecNotFound == moduleMap.size() ) {
                QLogger::QLog_Error("Cacic Daemon (Timer)", "Não foi possível encontrar nenhum módulo executável!");
                return QStringList();
            }

            // pula para o próximo módulo no moduloMap
            moduloIterator++;
            continue;
        }

        QString oldMd5 = QString(QCryptographicHash::hash(fileOld->readAll(),QCryptographicHash::Md5).toHex());
        if ( oldMd5 != hashModulo ) {

#if defined(Q_OS_WIN)
            fileOld->rename(this->applicationDirPath + "/" + nomeModulo + ".exe",
                            this->applicationDirPath + "/" + nomeModulo + "Old.exe");
#elif defined(Q_OS_LINUX)
            fileOld->rename(this->applicationDirPath + "/" + nomeModulo,
                            this->applicationDirPath + "/" + nomeModulo + "Old");
#endif

            // Download nova versão do executável
            QList<QMap<QString,QString> >::const_iterator metodosIterator = metodosDownload.constBegin();
            bool downloadSucess = false;
            while ( !downloadSucess && metodosIterator != metodosDownload.constEnd() ) {
                if( metodosIterator->value("tipo") == "ftp" || metodosIterator->value("tipo") == "" ) {
                    if ( OCacicComm->ftpDownload( metodosIterator->value("url"), metodosIterator->value("path") )  )
                        downloadSucess = true;
                } else if ( metodosIterator->value("tipo") == "http" ) {
                    if( OCacicComm->httpDownload( metodosIterator->value("url"), metodosIterator->value("path") )  )
                        downloadSucess = true;
                }
                metodosIterator++;
            }
            fileOld->remove();
            delete fileOld;
        }

        nomesModulos.append(nomeModulo);

        moduloIterator++;
    }

    return nomesModulos;
}
