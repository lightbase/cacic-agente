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
    delete checkModules;
}

void CacicTimer::reiniciarTimer(){
    timer->stop();
    timer->start(getPeriodicidadeExecucao());
}


void CacicTimer::iniciarTimer(bool conexaoGerente)
{
    if(conexaoGerente){
        checkModules->start();
        verificarModulos();
        verificarPeriodicidade();
        timer->start(getPeriodicidadeExecucao());
    }else{
        timer->start(this->periodicidadeExecucaoPadrao * 60000);
    }

}

void CacicTimer::mslot(){
    if(comunicarGerente()){
        checkModules->start();
        verificarModulos();
        if (verificarEIniciarQMutex()) {
            iniciarThread();
            if(verificarPeriodicidade()){
                reiniciarTimer();
            }
        }
    }
}


bool CacicTimer::verificarEIniciarQMutex(){
    /*
     * para o refactoring: Quando matar o gercols travado na memoria imediatamente ja inicia outra coleta
     *
     */
    if(!cacicthread->isRunning()){
        cMutex->lock();
        QLogger::QLog_Info("Cacic Daemon (Timer)", "Semáforo fechado com sucesso.");
        return true;
    }else{
        QLogger::QLog_Info("Cacic Daemon (Timer)", "Possivelmente o gercols travou e será finalizado.");
        try{
            cacicthread->terminate();
            QLogger::QLog_Info("Cacic Daemon (Timer)", "Gercols finalizado com sucesso.");
        }catch (...){
            QLogger::QLog_Error("Cacic Daemon (Timer)", "Falha ao finalizar gercols.");
            return false;
        }
        cMutex->lock();
        QLogger::QLog_Info("Cacic Daemon (Timer)", "Semáforo fechado com sucesso.");
        return true;
    }
}

bool CacicTimer::verificarModulos()
{
    QDir dir("./temp");
    dir.setFilter(QDir::Files | QDir::Hidden | QDir::NoSymLinks | QDir::Executable);
    dir.setSorting(QDir::Size | QDir::Reversed);

    QFileInfoList list = dir.entryInfoList();
    for (int i = 0; i<list.size(); i++){
        if(!(list.at(i).fileName() == QString("cacic-service"))){
            QFile novoModulo(list.at(i).filePath());
            if (QFile::exists(QDir::currentPath() + "/" + list.at(i).fileName())){
                QFile::remove(QDir::currentPath() + "/" + list.at(i).fileName());
            }
            novoModulo.copy(QDir::currentPath() + "/" + list.at(i).fileName());
            novoModulo.close();
        }
    }
    return true;
}

void CacicTimer::iniciarThread(){
    //checkModules = new CheckModules(this->applicationDirPath);
    //checkModules->start();

    QString nome = "gercols";
    definirDirModulo(getApplicationDirPath(), nome);
    cacicthread->setCcacic(ccacic);
    cacicthread->setOCacicComm(OCacicComm);
    cacicthread->setNomeModulo(nome);
    cacicthread->setCMutex(cMutex);
    cacicthread->setModuloDirPath(getDirProgram());
    cacicthread->start(QThread::NormalPriority);
}

QString CacicTimer::getApplicationDirPath() {
    return applicationDirPath;
}

bool CacicTimer::comunicarGerente(){
    bool ok;
    QJsonObject resposta = OCacicComm->login(&ok);
    if(resposta.isEmpty() || resposta.contains("error")){
        //de vez enquando a conexão da erro, é bom tentar 2 vezes pra garantir.
        QLogger::QLog_Info("Cacic Daemon (Timer)", "Erro no primeiro login.");
        resposta = OCacicComm->login(&ok);
        if(resposta.isEmpty() || resposta.contains("error")){
            QLogger::QLog_Info("Cacic Daemon (Timer)", "Erro no segundo login.");
            return false;
        }
    }
    /*
     * no refactoring: Apenas fazer o login quando a sessão estiver expirada.
     *
     * */

    QLogger::QLog_Info("Cacic Daemon (Timer)", "getLogin() success.");
    if(getTest()){
        QLogger::QLog_Info("Cacic Daemon (Timer)", QString("getTeste() success."));
        if(getConfig()){
            QLogger::QLog_Info("Cacic Daemon (Timer)", QString("getConfig() success."));
            return true;
        } else{
            QLogger::QLog_Error("Cacic Daemon (Timer)", "Falha na execução do getConfig().");
            return false;
        }
    }else{
        QLogger::QLog_Error("Cacic Daemon (Timer)", "Falha na execução do getTest().");
        return false;
    }
}

bool CacicTimer::getTest(){
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
        return ok;
    } catch (...) {
        qDebug() << "Erro ao salvar o arquivo de configurações.";
        return false;
    }
}

bool CacicTimer::getConfig(){
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
                              this->applicationDirPath + "/getConfig.json");
        return ok;
    } catch (...) {
        qDebug() << "Erro ao salvar o arquivo de configurações.";
        return false;
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




void CacicTimer::iniciarInstancias(){
    logManager = QLogger::QLoggerManager::getInstance();
    logManager->addDestination(this->applicationDirPath + "/Logs/cacicLog.txt","Cacic Daemon (Timer)",QLogger::InfoLevel);
    logManager->addDestination(this->applicationDirPath + "/Logs/cacicLog.txt","Cacic Daemon (Timer)",QLogger::ErrorLevel);
    ccacic = new CCacic();
    timer = new QTimer(this);
    cMutex = new QMutex(QMutex::Recursive);
    cacicthread = new CacicThread(this->applicationDirPath);
    OCacicComm = new CacicComm();
    //OCacicComm->setUrlSsl();
    checkModules = new CheckModules(this->applicationDirPath);
    OCacicComm->setUrlGerente(ccacic->getValueFromRegistry("Lightbase", "Cacic", "applicationUrl").toString());
    OCacicComm->setUsuario(ccacic->getValueFromRegistry("Lightbase", "Cacic", "usuario").toString());
    OCacicComm->setPassword(ccacic->getValueFromRegistry("Lightbase", "Cacic", "password").toString());
    ccacic->setChaveCrypt(ccacic->getValueFromRegistry("Lightbase", "Cacic", "key").toString());
}

bool CacicTimer::verificarPeriodicidade()
{
    QJsonObject agenteConfigJson;
    QJsonObject configuracoes;
    QJsonObject result = ccacic->getJsonFromFile(this->applicationDirPath + "/getConfig.json");
    if(!result.contains("error") && !result.isEmpty()){
        agenteConfigJson = result["agentcomputer"].toObject();
        configuracoes = agenteConfigJson["configuracoes"].toObject();
        if(getPeriodicidadeExecucao() != configuracoes["nu_intervalo_exec"].toString().toInt()){
            setPeriodicidadeExecucao(configuracoes["nu_intervalo_exec"].toString().toInt() * 60000);
            return true;
        }
    }else{
        setPeriodicidadeExecucao(this->periodicidadeExecucaoPadrao * 60000);
        QLogger::QLog_Error("Cacic Daemon (Timer)", QString("getConfig.json com erro ou vazio"));
        return false;
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
