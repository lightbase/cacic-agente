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
        comunicarGerente();
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
            QLogger::QLog_Info("Cacic Daemon (Timer)", "Módulo " + list.at(i).fileName() + " encontrado para atualização.");
            QFile novoModulo(list.at(i).filePath());
            if (QFile::exists(applicationDirPath + "/" + list.at(i).fileName())){
                QLogger::QLog_Info("Cacic Daemon (Timer)", "Excluindo versão antiga de "+list.at(i).fileName());
                QFile::remove(applicationDirPath + "/" + list.at(i).fileName());
            }
            novoModulo.copy(applicationDirPath + "/" + list.at(i).fileName());
            if (!novoModulo.remove())
                QLogger::QLog_Info("Cacic Daemon (Timer)", "Falha ao excluir "+list.at(i).fileName()+" da pasta temporária.");
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
        QLogger::QLog_Info("Cacic Daemon (Timer)", "Erro no primeiro login. Código: " + resposta["error"].toString());
        resposta = OCacicComm->login(&ok);
        if(resposta.isEmpty() || resposta.contains("error")){
            QLogger::QLog_Info("Cacic Daemon (Timer)", "Erro no segundo login." + resposta["error"].toString());
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
            return false;
        }
    }else{
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
        QLogger::QLog_Error("Cacic Daemon (Timer)", "Falha na execução do getTest()." + jsonresult["error"].toString());
        return false;
    }
    try{
        ccacic->setJsonToFile(jsonresult.contains("reply") ? jsonresult["reply"].toObject() : jsonresult,
                              ccacic->getCacicMainFolder() + "/getTest.json");
        return ok;
    } catch (...) {
        QLogger::QLog_Error("Cacic Daemon (Timer)","Erro ao salvar o arquivo de configurações.");
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
        QLogger::QLog_Error("Cacic Daemon (Timer)", "Falha na execução do getConfig()." + jsonresult["error"].toString());
        return false;
    }
    try{
        ccacic->setJsonToFile(jsonresult.contains("reply") ? jsonresult["reply"].toObject() : jsonresult,
                              ccacic->getCacicMainFolder() + "/getConfig.json");
        return ok;
    } catch (...) {
        QLogger::QLog_Error("Cacic Daemon (Timer)","Erro ao salvar o arquivo de configurações.");
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
    logManager->addDestination(this->applicationDirPath + "/Logs/cacic.log","Cacic Daemon (Timer)",QLogger::InfoLevel);
    logManager->addDestination(this->applicationDirPath + "/Logs/cacic.log","Cacic Daemon (Timer)",QLogger::ErrorLevel);
    ccacic = new CCacic();
    ccacic->setCacicMainFolder(applicationDirPath);
    timer = new QTimer(this);
    cMutex = new QMutex(QMutex::Recursive);
    cacicthread = new CacicThread(this->applicationDirPath);
    OCacicComm = new CacicComm();
    //OCacicComm->setUrlSsl();
    checkModules = new CheckModules(this->applicationDirPath, "Cacic Daemon (Timer)");
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
