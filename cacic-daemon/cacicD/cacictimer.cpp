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

    //Caso verifique que a thread ainda está em execução e não consiga finalizá-la.
    //Acredito que seja difícil acontecer, mas vai que...
    if (verificarEIniciarQMutex()) {
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
            }else{
                QLogger::QLog_Error("Cacic Daemon (Timer)", "Falha na obtenção do arquivo de configuração.");
            }
        }else{
            QLogger::QLog_Error("Cacic Daemon (Timer)", "Falha na execução do getTest().");
        }
    }
}


bool CacicTimer::verificarEIniciarQMutex(){
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
            return ok;
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
            return ok;
        } catch (...) {
            qDebug() << "Erro ao salvar o arquivo de configurações.";
            return false;
        }
    } catch (...){
        qDebug() << "Erro ao conectar com o servidor.";
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
