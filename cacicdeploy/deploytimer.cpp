#include "deploytimer.h"

deployTimer::deployTimer(QObject *parent) :
    QObject(parent)
{
}

deployTimer::deployTimer(QString cacicFolder){
    this->cacicFolder = cacicFolder;
    log = new LogCacic(LOG_CACICDEPLOY, this->cacicFolder+"/Logs");
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
    QJsonArray outrosModulos = CCacic::getJsonFromFile(this->cacicFolder + "/getConfig.json")
                                                    ["agentcomputer"].toObject()["modulos"].toObject()
                                                    ["outros"].toArray();
    if (!outrosModulos.empty()) {
        for (int i = 0; i < outrosModulos.size(); i++) {
            //Se a data/hora de execução for menor que a data/hora atual, segue para a próxima etapa, que é autorização.
            if (QDateTime::fromString(outrosModulos.at(i).toObject()["dataExecucao"].toString(), "dd/mm/yyyy hh:mm:ss").
                    secsTo(QDateTime::currentDateTime()) < 0){
                if (commExecucao(outrosModulos.at(i).toObject(), ROTA_AUTORIZA)){
                    //executa módulo
                }
            }

        }
    }
    log->escrever(LogCacic::InfoLevel, "OnTimer");
}

bool deployTimer::commExecucao(QJsonObject modulo, QString rota, bool statusExec)
{
    QJsonObject jsonComm;
    CACIC_Computer oComputer;
    jsonComm["modulo"] = modulo;
    jsonComm["data"] = QDateTime::currentDateTime().toString("dd/MM/yyyy hh:mm:ss");
    jsonComm["computador"] = oComputer.toJsonObject();

    //se for pra confirmação, deve ser adicionado a variável abaixo
    if (rota == ROTA_CONFIRMA) {
        jsonComm["statusExec"] = statusExec;
    }
    CacicComm comm;
    comm.setUrlGerente(CCacic::getValueFromRegistry("Lightbase", "Cacic", "applicationUrl").toString());
    if (comm.getUrlGerente().isEmpty()){
        QString urlGerente = CCacic::getJsonFromFile(this->cacicFolder+"/getConfig.json")
                                ["agentcomputer"].toObject()["applicationUrl"].toString();
        if (!urlGerente.isEmpty() && !urlGerente.isNull())
            comm.setUrlGerente(urlGerente);
        else {
            log->escrever(LogCacic::InfoLevel, "Erro durante tentativa de comunicação.");
            log->escrever(LogCacic::ErrorLevel, "Url do cacic não encontrada. Para configurar novamente, execute o install-cacic com a opção -configure");
            return false;
        }
    }
    comm.setUsuario(CCacic::getValueFromRegistry("Lightbase", "Cacic", "usuario").toString());
    comm.setPassword(CCacic::getValueFromRegistry("Lightbase", "Cacic", "password").toString());
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


