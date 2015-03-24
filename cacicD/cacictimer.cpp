#include "cacictimer.h"

CacicTimer::CacicTimer(QString dirpath)
{
    setApplicationDirPath(dirpath);
    iniciarInstancias();
    logcacic = new LogCacic(LOG_DAEMON_TIMER, dirpath+"/Logs");
    connect(timer,SIGNAL(timeout()),this,SLOT(mslot()));
}

CacicTimer::~CacicTimer()
{
    logcacic->~LogCacic();
}

void CacicTimer::reiniciarTimer(){
    timer->stop();
    timer->start(getPeriodicidadeExecucao());
    
    if(QString::number(getPeriodicidadeExecucaoAnterior() / 60000) > 0 &&
            !QString::number(getPeriodicidadeExecucaoAnterior() / 60000).isNull()){
        logcacic->escrever(LogCacic::InfoLevel, QString("Periodicidade de execução atualizada de: " +
                                                        QString::number(getPeriodicidadeExecucaoAnterior() / 60000) +
                                                        " minutos, para: " + QString::number(getPeriodicidadeExecucao() / 60000) +
                                                        " minutos."));
    }else{
        logcacic->escrever(LogCacic::InfoLevel, QString("Periodicidade de execução atualizada para: " +
                                                        QString::number(getPeriodicidadeExecucao() / 60000) + " minutos."));
    }
}


void CacicTimer::iniciarTimer()
{
    ccacic->salvarVersao("cacic-service");
    //iniciar em 2 minutos devido à placa de rede que às vezes não sobe à tempo.
    setPeriodicidadeExecucao(2 * 60000);
    timer->start(getPeriodicidadeExecucao());
}

//Slot que será iniciado sempre der a contagem do timer.
void CacicTimer::mslot(){
    if(comunicarGerente()){
        if ( QFile(ccacic->getCacicMainFolder() + "/cacic280.exe" ).exists() ||
             QFile(ccacic->getCacicMainFolder() + "/cacic260.exe" ).exists() ) {
            bool cacicRemovido;
            cacicRemovido = this->removeCacicAnterior();
            cacicRemovido = cacicRemovido && this->removeArquivosEstrangeiros(QDir(ccacic->getCacicMainFolder()));
            if(!cacicRemovido) {
                logcacic->escrever(LogCacic::ErrorLevel, QString("Problemas ao remover arquivos não pertencentes a esta versão do Cacic."));
            }
        }

        //Verifica atualizações.
        CheckModules *checkModules;
        checkModules = new CheckModules(this->applicationDirPath, LOG_DAEMON_TIMER);
        if (!checkModules->start()){
            logcacic->escrever(LogCacic::ErrorLevel, QString("Problemas ao checkar módulos."));
        }

        if (verificarEIniciarQMutex()) {
            iniciarThread();
            verificarModulos();
        }

        if(verificarPeriodicidade()){
            reiniciarTimer();
        }

    } else {
        logcacic->escrever(LogCacic::ErrorLevel, QString("Problemas ao comunicar com gerente."));
    }

}

/*************************************************************
 * Verifica se a thread ainda está rodando, se tiver mata ela.
 *************************************************************/
bool CacicTimer::verificarEIniciarQMutex(){
    /*
     * para o refactoring: Quando matar o gercols travado na memoria imediatamente ja inicia outra coleta
     *
     */
    if(!cacicthread->isRunning()){
        cMutex->lock();
        return true;
    }else{
        logcacic->escrever(LogCacic::InfoLevel, "Possivelmente o gercols travou e será finalizado.");
        try{
            cacicthread->terminate();
        }catch (...){
            logcacic->escrever(LogCacic::ErrorLevel, "Falha ao finalizar gercols.");
            return false;
        }
        cMutex->lock();
        return true;
    }
}
/**********************************************************************************************
 * Verifica se há algum módulo na pasta temporária, se tiver remove o antigo da pasta principal
 * e substitui pelo novo
 **********************************************************************************************/
bool CacicTimer::verificarModulos()
{
    QDir dir(ccacic->getCacicMainFolder() + "/temp");
    dir.setFilter(QDir::Files | QDir::Hidden | QDir::NoSymLinks | QDir::Executable);
    dir.setSorting(QDir::Size | QDir::Reversed);

    QFileInfoList list = dir.entryInfoList();
    for (int i = 0; i<list.size(); i++){
        if(!(list.at(i).fileName().contains("cacic-service"))){
            //verificar se há alguma thread sendo executada no momento.
            if (cacicthread->isRunning()){
                logcacic->escrever(LogCacic::InfoLevel, "Há uma thread sendo executada, aguardando o término.");
                if (!cacicthread->wait(30000)){
                    cacicthread->terminate();
                }
            }
            //Se o módulo for install-cacic, deverá ficar na pasta "/bin"
            QFile novoModulo(list.at(i).filePath());
            if (QFile::exists(applicationDirPath + "/" + (list.at(i).fileName().contains("install-cacic") ?
                                                         "bin/" + list.at(i).fileName() :
                                                         list.at(i).fileName()))){
                QFile::remove(applicationDirPath + "/" + (list.at(i).fileName().contains("install-cacic") ?
                                                              "bin/" + list.at(i).fileName() :
                                                               list.at(i).fileName()));
                //Garante a exclusão. às vezes o SO demora a reconhecer, dunno why.
                QThread::sleep(1);
            }

            if (!QFile::exists(applicationDirPath + "/" + (list.at(i).fileName().contains("install-cacic") ?
                                                           "bin/" + list.at(i).fileName() :
                                                            list.at(i).fileName()))){
                novoModulo.copy(applicationDirPath + "/" + (list.at(i).fileName().contains("install-cacic") ?
                                                            "bin/" + list.at(i).fileName() :
                                                             list.at(i).fileName()));
                if (!novoModulo.remove())
                    logcacic->escrever(LogCacic::ErrorLevel, "Falha ao excluir "+list.at(i).fileName()+" da pasta temporária.");
                else
                    logcacic->escrever(LogCacic::InfoLevel, "Módulo \"" + list.at(i).filePath() + "\" atualizado.");
            } else {
                logcacic->escrever(LogCacic::ErrorLevel, "Falha ao excluir módulo antigo"+list.at(i).fileName()+" da pasta temporária.");
            }

            novoModulo.close();
        } else {
            logcacic->escrever(LogCacic::InfoLevel, "Atualização do serviço necessária.");
            QStringList arg;
            arg << "-updateService";
            QProcess installCacicProc;
            installCacicProc.startDetached(ccacic->getCacicMainFolder() + "/install-cacic", arg);

#ifdef Q_OS_LINUX
            ConsoleObject console;
            console("killall -eq cacic-service");
            console("/etc/init.d/cacic3 stop");
#endif
            logcacic->~LogCacic();
            emit finalizar();
            break;
        }
    }
    list.clear();
    return true;
}

bool CacicTimer::verificarseModuloJaFoiExecutado(QString nome, QString hash){
    return ccacic->getValueFromRegistry("Lightbase", "Cacic", nome).toString() == hash;
}

int CacicTimer::getPeriodicidadeExecucaoAnterior() const
{
    return periodicidadeExecucaoAnterior;
}

void CacicTimer::setPeriodicidadeExecucaoAnterior(int value)
{
    periodicidadeExecucaoAnterior = value;
}


void CacicTimer::iniciarThread(){    
    QJsonObject agenteConfigJson;
    QJsonArray listaModulos;
    QJsonObject result = ccacic->getJsonFromFile(this->applicationDirPath + "/getConfig.json");
    if(!result.contains("error") && !result.isEmpty()){
        agenteConfigJson = result["agentcomputer"].toObject();
        listaModulos = agenteConfigJson["modulos"].toObject()["cacic"].toArray();
        QVariantMap modulosExecutados;
        for (int var = 0; var < listaModulos.size(); var++) {
            QString nome = listaModulos.at(var).toObject().value("nome").toString();
            QString hash = listaModulos.at(var).toObject().value("hash").toString();
            definirDirModulo(getApplicationDirPath(), nome);
            //Se não for cacic-service, nem install cacic, nem mapa OU Se for Mapa, não tiver sido
            //executado E o patrimônio estiver executado, iniciar a thread.
            if((!nome.contains("cacic-service")  &&
                !nome.contains("install-cacic")  &&
                !nome.contains("mapacacic")         ) ||
                    (nome.contains("mapacacic") &&
                     ccacic->getValueFromRegistry("Lightbase", "Cacic", nome).isNull() &&
                     agenteConfigJson["actions"].toObject()["col_patr"].toBool())){
                if (QFile::exists(getDirProgram())) {
                    cacicthread->setCcacic(ccacic);
                    cacicthread->setNomeModulo(nome);
                    cacicthread->setCMutex(cMutex);
                    cacicthread->setModuloDirPath(getDirProgram());
                    cacicthread->start(QThread::NormalPriority);
                    cacicthread->wait();
                    if(nome.contains("gercols")){
                        enviarColeta();
                        //enviarLogs(); ativar quando a parte do gerente estiver feita.
                    }
                    modulosExecutados[nome] = hash;
                    ccacic->setValueToRegistry("Lightbase", "Cacic", modulosExecutados);
                } else {
                    logcacic->escrever(LogCacic::InfoLevel, "Modulo \""+ nome + "\" não foi encontrado para execução.");
                }
            }
        }
    }
}

QString CacicTimer::getApplicationDirPath() {
    return applicationDirPath;
}

bool CacicTimer::comunicarGerente(){
    bool ok;
    CacicComm *OCacicComm;
    OCacicComm = new CacicComm();
    //Sempre recuperar as informações aqui caso mude.
    OCacicComm->setUrlGerente(ccacic->getValueFromRegistry("Lightbase", "Cacic", "applicationUrl").toString());
    OCacicComm->setUsuario(ccacic->getValueFromRegistry("Lightbase", "Cacic", "usuario").toString());
    OCacicComm->setPassword(ccacic->getValueFromRegistry("Lightbase", "Cacic", "password").toString());
    logcacic->escrever(LogCacic::InfoLevel, QString("Realizando comunicação em: " + OCacicComm->getUrlGerente()));
    ccacic->setChaveCrypt(ccacic->getValueFromRegistry("Lightbase", "Cacic", "key").toString());
    QJsonObject resposta = OCacicComm->login(&ok);
    if(resposta.isEmpty() || resposta.contains("error")){
        //de vez enquando a conexão da erro, é bom tentar 2 vezes pra garantir.
        resposta = OCacicComm->login(&ok);
        if(resposta.isEmpty() || resposta.contains("error")){
            logcacic->escrever(LogCacic::ErrorLevel, QString("Erro no login: " + resposta["error"].toString()));
            logcacic->escrever(LogCacic::InfoLevel, QString("Falha na tentativa de login."));
            return false;
        }
    }
    resposta = getTest(*OCacicComm);
    if(!resposta.contains("error")){
        resposta = getConfig(*OCacicComm);
        if(!resposta.contains("error")){
            return true;
        } else{
            logcacic->escrever(LogCacic::ErrorLevel, "Erro ao pegar informações do gerente: " + resposta["error"].toString());            return false;
            logcacic->escrever(LogCacic::InfoLevel, QString("Falha ao tentar pegar informações do gerente."));
        }
    }else{
        logcacic->escrever(LogCacic::InfoLevel, QString("Falha ao tentar pegar informações do gerente."));
        return false;
    }
}

QJsonObject CacicTimer::getTest(CacicComm &OCacicComm){
    bool ok;
    QJsonObject as;
    CACIC_Computer OCacic_Computer;
    as["computador"] = OCacic_Computer.toJsonObject();
    QJsonObject jsonresult = OCacicComm.comm(ROTA_GETTEST, &ok, as, true);
    if(!ok){
        jsonresult = OCacicComm.comm(ROTA_GETTEST, &ok, as, true); // mais uma vez pra garantir.
    }
    if(jsonresult.contains("error")){
        logcacic->escrever(LogCacic::ErrorLevel, "Falha na execução do getTest(). " + jsonresult["error"].toString());
        return jsonresult;
    }
    try{
        ccacic->setJsonToFile(jsonresult.contains("reply") ? jsonresult["reply"].toObject() : jsonresult,
                              ccacic->getCacicMainFolder() + "/getTest.json");
        return jsonresult;
    } catch (...) {
        logcacic->escrever(LogCacic::ErrorLevel, "Erro ao salvar o arquivo de configurações.");
        return jsonresult;
    }
}

QJsonObject CacicTimer::getConfig(CacicComm &OCacicComm){
    bool ok;
    QJsonObject as;
    CACIC_Computer OCacic_Computer;
    as["computador"] = OCacic_Computer.toJsonObject();
    QJsonObject jsonresult = OCacicComm.comm(ROTA_GETCONFIG, &ok, as, true);
    if(jsonresult.contains("error")){
        logcacic->escrever(LogCacic::ErrorLevel, "Falha na execução do getConfig()." + jsonresult["error"].toString());
        return jsonresult;
    }
    try{
        ccacic->setJsonToFile(jsonresult.contains("reply") ? jsonresult["reply"].toObject() : jsonresult,
                              ccacic->getCacicMainFolder() + "/getConfig.json");
        QVariantMap registro;
        registro["applicationUrl"] = QVariant::fromValue(ccacic->getJsonFromFile(ccacic->getCacicMainFolder() + "/getConfig.json")
                                                            ["agentcomputer"].toObject()
                                                            ["applicationUrl"].toString());
        ccacic->setValueToRegistry("Lightbase", "Cacic", registro);

        return jsonresult;
    } catch (...) {
        logcacic->escrever(LogCacic::ErrorLevel, "Erro ao salvar o arquivo de configurações.");
        return jsonresult;
    }
}

bool CacicTimer::verificaForcarColeta(){
    QJsonObject agenteConfigJson;
    QJsonObject configuracoes;
    QJsonObject result = ccacic->getJsonFromFile(this->applicationDirPath + "/getConfig.json");
    if(!result.contains("error") && !result.isEmpty()){
        agenteConfigJson = result["agentcomputer"].toObject();
        configuracoes = agenteConfigJson["configuracoes"].toObject();
        if(!configuracoes["nu_forca_coleta"].isNull()){
            if(configuracoes["nu_forca_coleta"].toString() == "true" || configuracoes["nu_forca_coleta"].toString() == "True"){
                return true;
            }
        }
    }
    return false;
}

bool CacicTimer::enviarColetaDiff(){
    if(QFile::exists(ccacic->getCacicMainFolder() + "/coletaDiff.json")){
        bool ok = false;
        QJsonObject jsonColeta = this->ccacic->getJsonFromFile(this->applicationDirPath + "/coletaDiff.json");
        if (!jsonColeta.isEmpty()){
            CacicComm *OCacicComm = new CacicComm();
            OCacicComm->setUrlGerente(ccacic->getValueFromRegistry("Lightbase", "Cacic", "applicationUrl").toString());
            OCacicComm->setUsuario(ccacic->getValueFromRegistry("Lightbase", "Cacic", "usuario").toString());
            OCacicComm->setPassword(ccacic->getValueFromRegistry("Lightbase", "Cacic", "password").toString());
            QJsonObject retornoColeta;
            logcacic->escrever(LogCacic::InfoLevel, QString("Enviando coleta Diff ao gerente."));
            retornoColeta = OCacicComm->comm(ROTA_COLETA_DIFF, &ok, jsonColeta , true);
            if(retornoColeta.contains("error")) {
                logcacic->escrever(LogCacic::ErrorLevel,  QString("Falha ao enviar a deferença de coleta: " + retornoColeta["error"].toString()));
            }
            return ok;
        } else {
            logcacic->escrever(LogCacic::ErrorLevel, QString("Falha ao ler a diferença de coleta: Arquivo JSON vazio ou inexistente."));
            return false;
        }
    }
    return false;
}

bool CacicTimer::realizarEnvioDeLog(LogCacic::CacicLogLevel level){
    bool ok = false;
    QJsonObject jsonColeta = this->ccacic->getJsonFromFile(logcacic->resolverEnderecoArquivo(level));
    if (!jsonColeta.isEmpty()){
        CacicComm *OCacicComm = new CacicComm();
        OCacicComm->setUrlGerente(ccacic->getValueFromRegistry("Lightbase", "Cacic", "applicationUrl").toString());
        OCacicComm->setUsuario(ccacic->getValueFromRegistry("Lightbase", "Cacic", "usuario").toString());
        OCacicComm->setPassword(ccacic->getValueFromRegistry("Lightbase", "Cacic", "password").toString());
        QJsonObject retornoColeta;
        logcacic->escrever(LogCacic::InfoLevel, QString("Enviando Log "+logcacic->getLevelEmString(level)+" ao gerente."));
        retornoColeta = OCacicComm->comm(ROTA_LOG, &ok, jsonColeta , true);
        if (ok){
            if(!retornoColeta.isEmpty() && !retornoColeta.contains("error")){
                logcacic->escrever(LogCacic::InfoLevel, "Log enviado com sucesso.");
                return true;
            } else if(retornoColeta.contains("error")) {
                logcacic->escrever(LogCacic::ErrorLevel, QString("Falha ao enviar log "
                                                                 +logcacic->getLevelEmString(level)
                                                                 + " para o gerente: " + retornoColeta["error"].toString()));
                return false;
            }
            return ok;
        } else {
            logcacic->escrever(LogCacic::ErrorLevel, QString("Falha ao enviar aquivo de log "
                                                             + logcacic->getLevelEmString(level)
                                                             + " para o gerente: Arquivo de Log vazio ou inexistente."));
            return false;
        }
    }
    return ok;
}

bool CacicTimer::enviarLogs(){
    if(QFile::exists(ccacic->getCacicMainFolder() + "/getConfig.json")){
        QJsonObject agenteConfigJson;
        QJsonObject logs;
        QJsonObject result = ccacic->getJsonFromFile(ccacic->getCacicMainFolder() + "/getConfig.json");
        if(!result.contains("error") && !result.isEmpty()){
            agenteConfigJson = result["agentcomputer"].toObject();
            logs = agenteConfigJson["logs"].toObject();
            if(!logs["ErrorLevel"].isNull()){
                if(logs["ErrorLevel"].toString() == "true" || logs["ErrorLevel"].toString() == "True"){
                    if(realizarEnvioDeLog(LogCacic::ErrorLevel)){
                        return true;
                    }else{
                        return false;
                    }
                }
            }
            if(!logs["FatalLevel"].isNull()){
                if(logs["FatalLevel"].toString() == "true" || logs["FatalLevel"].toString() == "True"){
                    if(realizarEnvioDeLog(LogCacic::FatalLevel)){
                        return true;
                    }else{
                        return false;
                    }
                }
            }if(!logs["DebugLevel"].isNull()){
                if(logs["DebugLevel"].toString() == "true" || logs["DebugLevel"].toString() == "True"){
                    if(realizarEnvioDeLog(LogCacic::DebugLevel)){
                        return true;
                    }else{
                        return false;
                    }
                }
            }if(!logs["InfoLevel"].isNull()){
                if(logs["InfoLevel"].toString() == "true" || logs["InfoLevel"].toString() == "True"){
                    if(realizarEnvioDeLog(LogCacic::InfoLevel)){
                        return true;
                    }else{
                        return false;
                    }
                }
            }if(!logs["TraceLevel"].isNull()){
                if(logs["TraceLevel"].toString() == "true" || logs["TraceLevel"].toString() == "True"){
                    if(realizarEnvioDeLog(LogCacic::TraceLevel)){
                        return true;
                    }else{
                        return false;
                    }
                }
            }if(!logs["WarnLevel"].isNull()){
                if(logs["WarnLevel"].toString() == "true" || logs["WarnLevel"].toString() == "True"){
                    if(realizarEnvioDeLog(LogCacic::WarnLevel)){
                        return true;
                    }else{
                        return false;
                    }
                }
            }
        }
    }
    return false;
}

bool CacicTimer::realizarEnviodeColeta(){
    bool ok = false;
    QJsonObject jsonColeta = this->ccacic->getJsonFromFile(this->applicationDirPath + "/coleta.json");
    if (!jsonColeta.isEmpty()){
        CacicComm *OCacicComm = new CacicComm();
        OCacicComm->setUrlGerente(ccacic->getValueFromRegistry("Lightbase", "Cacic", "applicationUrl").toString());
        OCacicComm->setUsuario(ccacic->getValueFromRegistry("Lightbase", "Cacic", "usuario").toString());
        OCacicComm->setPassword(ccacic->getValueFromRegistry("Lightbase", "Cacic", "password").toString());
        QJsonObject retornoColeta;
        logcacic->escrever(LogCacic::InfoLevel, QString("Enviando coleta ao gerente."));
        retornoColeta = OCacicComm->comm(ROTA_COLETA, &ok, jsonColeta , true);
        if (ok){
            if(!retornoColeta.isEmpty() && !retornoColeta.contains("error")){
                QVariantMap enviaColeta;
                enviaColeta["enviaColeta"] = false;
                ccacic->setValueToRegistry("Lightbase", "Cacic", enviaColeta);
            }
            logcacic->escrever(LogCacic::InfoLevel, "Coleta enviada com sucesso.");
            return true;
        } else if(retornoColeta.contains("error")) {
            logcacic->escrever(LogCacic::ErrorLevel, QString("Falha na coleta: " + retornoColeta["error"].toString()));
            return false;
        }
        return ok;
    } else {
        logcacic->escrever(LogCacic::ErrorLevel, QString("Falha na coleta: Arquivo JSON vazio ou inexistente."));
        return false;
    }
}

bool CacicTimer::enviarColeta() {
    if(QFile::exists(ccacic->getCacicMainFolder() + "/coleta.json")){
        if(!verificaForcarColeta()){
            if (ccacic->getValueFromRegistry("Lightbase", "Cacic", "enviaColeta").toBool()){
                if(realizarEnviodeColeta()){ // quando a ROTA_COLETA_DIFF existir no gerente, mudar para: enviarColetaDiff()
                    registrarDataEnvioDeColeta();
                    return true;
                }
                return false;
            }else{
                if(!ccacic->getValueFromRegistry("Lightbase", "Cacic", "dataColetaEnviada").toString().isEmpty()){
                    QDate dataUltimaColeta = QDate::fromString(ccacic->getValueFromRegistry("Lightbase", "Cacic", "dataColetaEnviada").toString(), "dd/MM/yyyy"); //recupera a data de envio do ultima coleta no formato "dayOfYears"
                    QDate dataAtual = QDate::currentDate(); //data atual no formato "dayOfYears"
                    if(dataUltimaColeta.daysTo(dataAtual) >= 7){ //se já tiver passado 7 dias desde o envio da ultima coleta, enviar mesmo sem alteração
                        if(realizarEnviodeColeta()){
                            registrarDataEnvioDeColeta();
                            return true;
                        }else{
                            return false;
                        }
                    }else{
                        logcacic->escrever(LogCacic::InfoLevel, QString("Sem diferença na coleta."));
                        return false;
                    }
                } else {
                    if(realizarEnviodeColeta()){
                        logcacic->escrever(LogCacic::InfoLevel, QString("Data da antiga coleta não encontrada, forçando o envio da coleta."));
                        registrarDataEnvioDeColeta();
                        return true;
                    }else{
                        return false;
                    }
                }
            }
        }else{
            if(realizarEnviodeColeta()){
                logcacic->escrever(LogCacic::InfoLevel, QString("Coleta forçada enviada com sucesso."));
                registrarDataEnvioDeColeta();
                return true;
            }
            return false;
        }
    }
    logcacic->escrever(LogCacic::InfoLevel, QString("Arquivo de coleta não encontrado."));
    return false;
}

void CacicTimer::registrarDataEnvioDeColeta(){
    QVariantMap qvm;
    qvm["dataColetaEnviada"] = QDate::currentDate().toString("dd/MM/yyyy");
    this->ccacic->setValueToRegistry("Lightbase", "Cacic", qvm);
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
    ccacic = new CCacic();
    ccacic->setCacicMainFolder(this->applicationDirPath);
    timer = new QTimer(this);
    cMutex = new QMutex(QMutex::Recursive);
    cacicthread = new CacicThread(this->applicationDirPath);
    ccacic->setChaveCrypt(ccacic->getValueFromRegistry("Lightbase", "Cacic", "key").toString());
}

bool CacicTimer::verificarPeriodicidade()
{
    QJsonObject configuracoes;
    QJsonObject result = ccacic->getJsonFromFile(this->applicationDirPath + "/getConfig.json");
    if(!result.contains("error") && !result.isEmpty()){
        configuracoes = result["agentcomputer"].toObject()["configuracoes"].toObject();
        QString tempoExec = configuracoes["nu_intervalo_exec"].toString();
        if((!tempoExec.isNull()) &&
                ((getPeriodicidadeExecucao() / 60000) != tempoExec.toInt())) {
            setPeriodicidadeExecucaoAnterior(this->getPeriodicidadeExecucao());
            setPeriodicidadeExecucao(tempoExec.toInt() * 60000);
            return true;
        } else if(tempoExec.isNull() || tempoExec.toInt() == 0) {
            setPeriodicidadeExecucao(this->periodicidadeExecucaoPadrao * 60000);
            logcacic->escrever(LogCacic::ErrorLevel, QString("valor do timer com erro ou vazio"));
            return false;
        }
    }else{
        setPeriodicidadeExecucao(this->periodicidadeExecucaoPadrao * 60000);
        logcacic->escrever(LogCacic::ErrorLevel, QString("getConfig.json com erro ou vazio"));
        return false;
    }
    return false;
}


bool CacicTimer::removeArquivosEstrangeiros(const QDir &diretorio)
{
    bool retorno = true;
    //    QDir dir(ccacic->getCacicMainFolder());
    QDir dir = diretorio;
    dir.setFilter(QDir::AllEntries | QDir::Hidden | QDir::NoDotAndDotDot );
    dir.setSorting(QDir::Size | QDir::Reversed);

    QFileInfoList list = dir.entryInfoList();
    for (int i = 0; i<list.size(); i++){
        // Lista arquivos a não serem excluídos
        if( !list.at(i).fileName().contains("cacic-service") &&
                !list.at(i).fileName().contains("cacic.log") &&
                !list.at(i).fileName().contains("install-cacic") &&
                !list.at(i).fileName().contains("gercols") &&
                !list.at(i).fileName().contains("getTest.json") &&
                !list.at(i).fileName().contains("getConfig.json") ) {
            if ( list.at(i).isDir()) {
                // Lista diretorios a não serem excluidos
                if( list.at(i).fileName() == "Logs" || list.at(i).fileName() == "temp" ) {
                    if( removeArquivosEstrangeiros(QDir(list.at(i).absoluteFilePath())) )
                        retorno = retorno && true;
                    else
                        retorno = retorno && false;
                } else if (!(list.at(i).fileName() == "bin")){
                    logcacic->escrever(LogCacic::InfoLevel, "Excluindo diretorio: " + list.at(i).fileName());
                    retorno = retorno && ccacic->deleteFolder(list.at(i).absoluteFilePath());
                }
            } else {
                retorno = retorno && ccacic->deleteFile(list.at(i).absoluteFilePath());
                logcacic->escrever(LogCacic::InfoLevel, "Excluindo arquivo: " + list.at(i).fileName());
            }
        }
    }
    return retorno;
}

bool CacicTimer::removeCacicAnterior(){
    bool retorno = true;
#if defined(Q_OS_WIN)
    ServiceController oldCacic(QString("cacicsustainservice").toStdWString());
    if(oldCacic.isInstalled()){
        if(!oldCacic.uninstall()){
            logcacic->escrever(LogCacic::ErrorLevel, "Falha ao excluir serviço do cacic 2.8: " +
                               QString::fromStdString(oldCacic.getLastError()));
        }
    }
    //Garante a pausa do serviço para que o windows não bloqueie a exclusão
    QThread::sleep(3);

    QStringList cacicFiles;
    cacicFiles << "chksis.inf" << "chksis.exe" << "cacicservice.exe";
    foreach(QString file, cacicFiles){
        if(QFile::exists("c:/windows/"+file)){
            retorno = retorno && ccacic->deleteFile(QDir::rootPath()+"windows/"+file);
        }
    }
#endif
    return retorno;
}

void CacicTimer::definirDirModulo(QString appDirPath, QString nome){
    setDirProgram(appDirPath + "/"+ nome);
}

int CacicTimer::getPeriodicidadeExecucao() const
{
    return periodicidadeExecucao;
}

void CacicTimer::setPeriodicidadeExecucao(int value)
{
    this->periodicidadeExecucao = value;
}