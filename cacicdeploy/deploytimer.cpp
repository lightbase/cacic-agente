#include "deploytimer.h"

deployTimer::deployTimer(QObject *parent) :
    QObject(parent)
{
}

deployTimer::deployTimer(CCacic *cacic){
    oCacic = cacic;
    log = new LogCacic(LOG_CACICDEPLOY, oCacic->getCacicMainFolder()+"/Logs");
    timerDeploy = new QTimer();
    QObject::connect(timerDeploy, SIGNAL(timeout()), this, SLOT(onTimer()));

    timerCheckService = new QTimer();
    QObject::connect(timerCheckService, SIGNAL(timeout()), this, SLOT(onTimerCheckService()));

}

bool deployTimer::start(int msecDeploy, int msecCheckService)
{
    //timer para fazer a verificação de deploy
    timerDeploy->setInterval(msecDeploy);
    timerDeploy->start();

    //timer para verificar se o serviço está instalado e rodando.
    timerCheckService->setInterval(msecCheckService);
    timerCheckService->start();

    //A primeira rotina do timer não é imediata, então é bom 'forçar' a primeira imediatamente.
    this->onTimer();
    this->onTimerCheckService();

    return true;
}

void deployTimer::onTimer()
{
    QJsonArray outrosModulos = oCacic->getJsonFromFile(oCacic->getCacicMainFolder() + "/getConfig.json")
                                                    ["agentcomputer"].toObject()["modulos"].toObject()
                                                    ["outros"].toArray();
    if (!outrosModulos.empty()) {
        for (int i = 0; i < outrosModulos.size(); i++) {
            //Se a data/hora de execução for menor que a data/hora atual, segue para a próxima etapa, que é autorização.
            if (QDateTime::fromString(outrosModulos.at(i).toObject()["dataExecucao"].toString(), "dd/mm/yyyy hh:mm:ss").
                    secsTo(QDateTime::currentDateTime()) < 0){
                if (commExecucao(ROTA_AUTORIZA)){
                    //executa módulo
                }
            }

        }
    }
    log->escrever(LogCacic::InfoLevel, "OnTimer");
}

bool deployTimer::commExecucao(QString rota, bool statusExec = false)
{
    QJsonObject jsonComm;
    CACIC_Computer oComputer;
    jsonComm["modulo"] = outrosModulos.at(i);
    jsonComm["data"] = QDateTime::currentDateTime().toString("dd/MM/yyyy hh:mm:ss");
    jsonComm["computador"] = oComputer.toJsonObject();

    //se for pra confirmação, deve ser adicionado a variável abaixo
    if (rota == ROTA_CONFIRMA) {
        jsonComm["statusExec"] = statusExec;
    }
    CacicComm comm;
    oCacic->setUrlGerente(oCacic->getValueFromRegistry("Lightbase", "Cacic", "applicationUrl"));
    if (oCacic->getUrlGerente().isEmpty()){
        QString urlGerente = oCacic->getJsonFromFile(oCacic->getCacicMainFolder()+"/getConfig.json")
                                ["agentcomputer"].toObject()["applicationUrl"].toString();
        if (!urlGerente.isEmpty() && !urlGerente.isNull())
            oCacic->setUrlGerente(urlGerente);
        else {
            log->escrever(LogCacic::InfoLevel, "Erro durante tentativa de comunicação.");
            log->escrever(LogCacic::ErrorLevel, "Url do cacic não encontrada. Para configurar novamente, execute "+
                                                "manualmente o install-cacic com a opção configure. (para maiores"+
                                                " informações digite install-cacic -h");
            return false;
        }
    }
    comm.setUrlGerente(urlGerente);
    comm.setUsuario(oCacic->getValueFromRegistry("Lightbase", "Cacic", "usuario"));
    comm.setPassword(oCacic->getValueFromRegistry("Lightbase", "Cacic", "password"));
    bool ok;
    jsonComm = comm.comm(rota, &ok, jsonComm, true);
    if (rota == ROTA_AUTORIZA)
        return !jsonComm["autoriza"].isNull() && jsonComm["autoriza"].toBool() && ok;
    else if (rota == ROTA_CONFIRMA)
        return ok;
    else
        return false;

}

void deployTimer::onTimerCheckService()
{
    log->escrever(LogCacic::InfoLevel, "OnTimerCheckService");
}


