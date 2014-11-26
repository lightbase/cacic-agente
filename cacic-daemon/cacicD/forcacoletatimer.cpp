#include "forcacoletatimer.h"

ForcaColetaTimer::ForcaColetaTimer(QString dirpath) :
    QObject(parent)
{
    setApplicationDirPath(dirpath);
    iniciarInstancias();
    connect(timer,SIGNAL(timeout()),this,SLOT(mslot()));
}

ForcaColetaTimer::~ForcaColetaTimer()
{
    logManager->closeLogger();
}

void ForcaColetaTimer::iniciarTimer()
{
    timer->start(this->periodicidadeExecucaoPadrao * 60000);
}

QJsonObject ForcaColetaTimer::getTest()
{

}

QJsonObject ForcaColetaTimer::getConfig()
{

}

bool ForcaColetaTimer::comunicarGerente()
{
    bool ok;
    //Sempre recuperar as informações aqui caso mude.
    OCacicComm->setUrlGerente(ccacic->getValueFromRegistry("Lightbase", "Cacic", "applicationUrl").toString());
    QLogger::QLog_Info("Cacic Daemon (ForcaColetaTimer)", "Realizando comunicação em: " + OCacicComm->getUrlGerente());
    OCacicComm->setUsuario(ccacic->getValueFromRegistry("Lightbase", "Cacic", "usuario").toString());
    OCacicComm->setPassword(ccacic->getValueFromRegistry("Lightbase", "Cacic", "password").toString());
    ccacic->setChaveCrypt(ccacic->getValueFromRegistry("Lightbase", "Cacic", "key").toString());

    QJsonObject resposta = OCacicComm->login(&ok);
    if(resposta.isEmpty() || resposta.contains("error")){
        //de vez enquando a conexão da erro, é bom tentar 2 vezes pra garantir.
        resposta = OCacicComm->login(&ok);
        if(resposta.isEmpty() || resposta.contains("error")){
            QLogger::QLog_Error("Cacic Daemon (ForcaColetaTimer)", "Erro no login: " + resposta["error"].toString());
            return false;
        }
    }

    QLogger::QLog_Info("Cacic Daemon (ForcaColetaTimer)", "getLogin() success.");
    resposta = getTest();
    if(!resposta.contains("error")){
        QLogger::QLog_Info("Cacic Daemon (ForcaColetaTimer)", QString("getTeste() success."));
        resposta = getConfig();
        if(!resposta.contains("error")){
            QLogger::QLog_Info("Cacic Daemon (ForcaColetaTimer)", QString("getConfig() success."));
            return true;
        } else{
            QLogger::QLog_Error("Cacic Daemon (ForcaColetaTimer)", "Erro ao pegar informações do gerente: " + resposta["error"].toString());
            return false;
        }
    }else{
        return false;
    }
}

void ForcaColetaTimer::reiniciarTimer()
{
    timer->stop();
    timer->start(this->periodicidadeExecucao);
}

ForcaColetaTimer::iniciarInstancias(){
    logManager = QLogger::QLoggerManager::getInstance();
    logManager->addDestination(this->applicationDirPath + "/Logs/cacic.log","Cacic Daemon (ForcaColetaTimer)",QLogger::InfoLevel);
    logManager->addDestination(this->applicationDirPath + "/Logs/cacic_error.log","Cacic Daemon (ForcaColetaTimer)",QLogger::ErrorLevel);
    timer = new QTimer(this);
    cMutex = new QMutex(QMutex::Recursive);
    ccacic = new CCacic();
    ccacic->setCacicMainFolder(this->applicationDirPath);
    OCacicComm = new CacicComm();
    OCacicComm->setUrlGerente(ccacic->getValueFromRegistry("Lightbase", "Cacic", "applicationUrl").toString());
    OCacicComm->setUsuario(ccacic->getValueFromRegistry("Lightbase", "Cacic", "usuario").toString());
    OCacicComm->setPassword(ccacic->getValueFromRegistry("Lightbase", "Cacic", "password").toString());
    ccacic->setChaveCrypt(ccacic->getValueFromRegistry("Lightbase", "Cacic", "key").toString());
}

void ForcaColetaTimer::lerArquivoConfig(const QJsonObject &jsonConfig)
{

}

bool ForcaColetaTimer::verificarPeriodicidade()
{
    QJsonObject agenteConfigJson;
    QJsonObject configuracoes;
    QJsonObject result = ccacic->getJsonFromFile(this->applicationDirPath + "/getConfig.json");
    if(!result.contains("error") && !result.isEmpty()){
        agenteConfigJson = result["agentcomputer"].toObject();
        configuracoes = agenteConfigJson["configuracoes"].toObject();
        if(!configuracoes["nu_intervalo_forca_coleta"].isNull()){
            setPeriodicidadeExecucao(configuracoes["nu_intervalo_forca_coleta"].toString().toInt() * 60000);
            return true;
        } else {
            setPeriodicidadeExecucao(this->periodicidadeExecucaoPadrao * 60000);
            QLogger::QLog_Error("Cacic Daemon (ForcaColetaTimer)", QString("valor do timer com erro ou vazio"));
            return false;
        }
    }else{
        setPeriodicidadeExecucao(this->periodicidadeExecucaoPadrao * 60000);
        QLogger::QLog_Error("Cacic Daemon (ForcaColetaTimer)", QString("getConfig.json com erro ou vazio"));
        return false;
    }
}

void ForcaColetaTimer::setPeriodicidadeExecucao(int value)
{
    periodicidadeExecucao = value;
}


void ForcaColetaTimer::setApplicationDirPath(const QString &value)
{
    applicationDirPath = value;
}

