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
    this->timeout = 0;

    this->mutex = new QMutex();
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
    log->escrever(LogCacic::InfoLevel, "Iniciando timer");
    QJsonArray outrosModulos = CCacic::getJsonFromFile(this->cacicFolder + "/getConfig.json")
                                                    ["agentcomputer"].toObject()["modulos"].toObject()
                                                    ["outros"].toArray();
    QJsonArray modulesToExec;
    if (!outrosModulos.empty()) {
        log->escrever(LogCacic::InfoLevel, "verificando data hora dos modulos");
        for (int i = 0; i < outrosModulos.size(); i++) {
            log->escrever(LogCacic::InfoLevel, "Modulo "+ outrosModulos.at(i).toObject()["nome"].toString()+
                                               " Data: "+ outrosModulos.at(i).toObject()["dataExecucao"].toString());

            log->escrever(LogCacic::InfoLevel, QString::number(QDateTime::fromString(outrosModulos.at(i).toObject()["dataExecucao"].toString(), "dd'/'MM'/'yyyy' 'HH:mm:ss").
                    secsTo(QDateTime::currentDateTime())));
            //Se a data/hora de execução for menor que a data/hora atual, segue para a próxima etapa, que é autorização.
            if (QDateTime::currentDateTime().secsTo(
                        QDateTime::fromString(outrosModulos.at(i).toObject()
                                              ["dataExecucao"].toString(), "dd'/'MM'/'yyyy' 'HH:mm:ss")) < 0){
                modulesToExec.append(outrosModulos.at(i));
            }
        }
        for (int i = 0; i < modulesToExec.size(); i++) {
            if (commExecucao(outrosModulos.at(i).toObject(), ROTA_AUTORIZA) || true){
                log->escrever(LogCacic::InfoLevel, "Iniciando módulo");
                QString nome = outrosModulos.at(i).toObject()["nome"].toString();
                QString hash = outrosModulos.at(i).toObject()["hash"].toString();
                int timeout  = outrosModulos.at(i).toObject()["timeout"].toInt();
                QFile modulo(this->cacicFolder + "deploy/" + nome);
                //verificar existência e concistência do módulo

                if(!modulo.exists() || (modulo.open(QIODevice::ReadOnly) && !CCacic::Md5IsEqual(modulo.readAll(), hash))){
                    log->escrever(LogCacic::InfoLevel, "Verificando hash e consistencia do módulo");
                    modulo.close();
                    if (!downloadModulo(nome)) {
                        log->escrever(LogCacic::InfoLevel, QString("Falha ao baixar " + nome));
                        return;
                    }
                } else if (modulo.exists()) {
                    if (modulo.size() < 1){
                        if (modulo.remove()){
                            if (!downloadModulo(nome)) {
                                log->escrever(LogCacic::InfoLevel, QString("Falha ao baixar " + nome));
                                return;
                            }
                        } else {
                            modulo.close();
                            log->escrever(LogCacic::ErrorLevel, "Não foi possível remover módulo antigo durante tentativa de atualização");
                            return;
                        }
                    }
                }
                this->timerDeploy->stop();
                if (i >= 1) {
                    //Confirmar se funciona se o objeto já estiver null;
                    if (thread != NULL) {
                        thread->isRunning();
                        thread->wait();
                    }
                }
                log->escrever(LogCacic::InfoLevel, "Iniciando thread");

                //se o timeout for 0, quer dizer que é a primeira thread a ser iniciada.
                if (this->timeout != 0) {
                    log->escrever(LogCacic::InfoLevel, "timeout diferente de 0");
                    if (!this->mutex->tryLock(this->timeout*1000)) {
                        log->escrever(LogCacic::ErrorLevel, "Falha ao parar semáforo.");
                        //se não conseguir travar o mutex, não deve continar.
                        this->timerDeploy->start();
                        return;
                    }
                } else {
                    log->escrever(LogCacic::InfoLevel, "Tentando parar mutex");
                    this->mutex->lock();
                }

                log->escrever(LogCacic::InfoLevel, "Criando objeto..");
                thread = new CacicThread(this->cacicFolder);

                //Conecto o finished() ao slot confirmaExecucao a baixo para não ser necessário causar dead lock nessa classe.
                connect(thread, SIGNAL(endExecution()), this, SLOT(confirmaExecucao()));
                log->escrever(LogCacic::InfoLevel, "Slots e signals conectados");
                thread->setCMutex(this->mutex);
                thread->setNomeModulo(nome);
                thread->setModuloDirPath(this->cacicFolder + "deploy/" + nome);
                thread->setTimeoutSec(timeout);
                this->timeout = timeout;
                this->moduloExec = outrosModulos.at(i).toObject();
                thread->start(QThread::NormalPriority);
                log->escrever(LogCacic::InfoLevel, "Iniciado");
            }
        }
    }
}

void deployTimer::confirmaExecucao()
{
    QProcess::ExitStatus exitStatusProc = thread->getLastStatus();
    log->escrever(LogCacic::InfoLevel, "Fim da execução do módulo ");
    if (exitStatusProc == QProcess::CrashExit) {
        log->escrever(LogCacic::ErrorLevel, "Fim da execução do módulo com código de erro: " + thread->getLastError());
    }

    if (!this->commExecucao(this->moduloExec, ROTA_CONFIRMA, exitStatusProc == QProcess::NormalExit)){
        log->escrever(LogCacic::ErrorLevel, "Falha ao confirmar execução.");
        //Fazer algo pra não executar o módulo novamente.
    }
    if(!QFile::remove(this->cacicFolder + "deploy/" + this->moduloExec["nome"].toString()))
        log->escrever(LogCacic::ErrorLevel, "Falha ao excluir " + this->moduloExec["nome"].toString());
    this->timerDeploy->start();
}


int deployTimer::getTimeout() const
{
    return timeout;
}

void deployTimer::setTimeout(int value)
{
    timeout = value;
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
    CacicComm comm(LOG_CACICDEPLOY, this->cacicFolder);
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

bool deployTimer::downloadModulo(QString nome)
{
    QJsonObject metodoDownload = CCacic::getJsonFromFile(this->cacicFolder+"getConfig.json")["agentcomputer"].toObject()
                                 ["metodoDownload"].toObject();
    CacicComm comm(LOG_CACICDEPLOY, this->cacicFolder);
    comm.setUrlGerente(CCacic::getValueFromRegistry("Lightbase", "Cacic", "applicationUrl").toString());
    if (comm.getUrlGerente().isEmpty()){
        QString urlGerente = CCacic::getJsonFromFile(this->cacicFolder+"getConfig.json")
                                ["agentcomputer"].toObject()["applicationUrl"].toString();
        if (!urlGerente.isEmpty() && !urlGerente.isNull())
            comm.setUrlGerente(urlGerente);
        else {
            log->escrever(LogCacic::InfoLevel, "Erro durante tentativa de comunicação.");
            log->escrever(LogCacic::ErrorLevel, "Url do cacic não encontrada. Para configurar novamente, execute o install-cacic com a opção -configure");
            return false;
        }
    }
    comm.setFtpPass(metodoDownload["senha"].toString());
    comm.setFtpUser(metodoDownload["usuario"].toString());
    log->escrever(LogCacic::InfoLevel, "comunicando");
    bool retorno = comm.fileDownload(metodoDownload["tipo"].toString(),
                                     comm.getUrlGerente(),
                                     metodoDownload["path"].toString() +
                                        (metodoDownload["path"].toString().endsWith("/") ? nome : "/" + nome),
                                     this->cacicFolder + "deploy/");
    return retorno;
}

void deployTimer::onTimerCheckService()
{
//    log->escrever(LogCacic::InfoLevel, "OnTimerCheckService");
}


