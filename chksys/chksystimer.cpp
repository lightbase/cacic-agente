#include "chksystimer.h"

chksysTimer::chksysTimer(QObject *parent) :
    QObject(parent)
{
}

chksysTimer::chksysTimer(QString cacicFolder){
    this->cacicFolder = cacicFolder;
    log = new LogCacic(LOG_CHKSYS, this->cacicFolder+"/Logs");

    timerCheckService = new QTimer();
    QObject::connect(timerCheckService, SIGNAL(timeout()), this, SLOT(onTimerCheckService()));
}

bool chksysTimer::start(int msecCheckService)
{

    //timer para verificar se o serviço está instalado e rodando.
    timerCheckService->setInterval(msecCheckService);
    timerCheckService->start();

    //A primeira rotina do timer não é imediata, então é bom 'forçar' a primeira imediatamente.
    this->onTimerCheckService();

    return true;
}

void chksysTimer::onTimerCheckService()
{
#ifdef Q_OS_WIN
    QFile fileService(cacicFolder+"/cacic-service.exe");
    ServiceController service(QString(CACIC_SERVICE_NAME).toStdWString());
    //Tenta instalar o serviço
    if (!service.isInstalled()){
        if ((!fileService.exists() || !fileService.size() > 0)) {
            log->escrever(LogCacic::ErrorLevel, "Não foi possível localizar o módulo do cacicdaemon.");
            fileService.close();

            this->downloadService();
        }
        log->escrever(LogCacic::InfoLevel, "Reinstalando serviço.");

        if (!service.install(QString(cacicMainFolder+"/cacic-service.exe").toStdWString(),
                             QString("Cacic Daemon").toStdWString())){
            log->escrever(LogCacic::ErrorLevel, "Falha ao reinstalar o serviço: " +
                                                                    QString::fromStdString(service.getLastError()));
        }
    }
    if (!service.isRunning()){
        if (service.start()){
            log->escrever(LogCacic::InfoLevel, QString("Serviço iniciado."));
        } else {
            log->escrever(LogCacic::ErrorLevel, "Falha ao iniciar o serviço: " +
                                                                    QString::fromStdString(service.getLastError()));
        }
    }
#else
    QFile fileService(cacicFolder+"/cacic-service");
    if ((!fileService.exists() || !fileService.size() > 0)) {
        log->escrever(LogCacic::ErrorLevel, "Não foi possível logalizar o módulo do cacicdaemon.");
        fileService.close();

        this->downloadService();
    } else {
        std::cout << "Iniciando serviço...\n";
        ConsoleObject console;
        std::cout << console("/etc/init.d/cacic3 start").toStdString();
    }
#endif
}

bool chksysTimer::downloadService()
{
    QFile *modulo;
    bool downloadOk = false;
    //pega o arquivo do módulo selecionado

#ifdef Q_OS_WIN
    QString moduloName = "cacic-service.exe";
#else
    QString moduloName = "cacic-service";
#endif
    modulo = new QFile(this->cacicFolder + moduloName);
    modulo->open(QFile::ReadOnly);

    QJsonObject metodoDownload;
    //verifica o tipo de download e tenta baixar o módulo para a pasta temporária.
    metodoDownload = CCacic::getJsonFromFile(this->cacicFolder + "/getConfig.json")
            ["agentcomputer"].toObject()
            ["metodoDownload"].toObject();
    if (!metodoDownload.isEmpty()){
        //verifica se já possuía o módulo atualizado na pasta temporária, se não baixa um novo.
        if (!(modulo->exists() && modulo->size()>1)){
            if (modulo->exists()) {
                if (!modulo->remove()){
                    log->escrever(LogCacic::ErrorLevel, "Falha ao remover módulo corrompido.");
                }
            }

            CacicComm *oCacicComm = new CacicComm(LOG_CHKSYS, this->cacicFolder);
            oCacicComm->setFtpUser(metodoDownload["usuario"].toString());
            oCacicComm->setFtpPass(metodoDownload["senha"].toString());

            downloadOk = oCacicComm->fileDownload(metodoDownload["tipo"].toString(),
                    metodoDownload["url"].toString(),
                    metodoDownload["path"].toString() +
                    (metodoDownload["path"].toString().endsWith("/") ? moduloName : "/" + moduloName),
                    this->cacicFolder);

            if (downloadOk){
                QFile *novoModulo;
                novoModulo = new QFile(this->cacicFolder + moduloName);

                //faz uma verificação do novo módulo.
                if (!(novoModulo->exists() && novoModulo->size()>1)){
                    log->escrever(LogCacic::ErrorLevel,
                                       QString("Falha ao baixar " + moduloName +
                                               "("+metodoDownload["tipo"].toString()+ "://" +
                                       metodoDownload["url"].toString() + metodoDownload["path"].toString() +
                            (metodoDownload["path"].toString().endsWith("/") ? moduloName : "/" + moduloName)+")"));
                    novoModulo->remove();
                    return false;
                } else {
                    log->escrever(LogCacic::InfoLevel, QString(moduloName + " baixado com sucesso!"));
                    return true;
                }
            } else {
                log->escrever(LogCacic::ErrorLevel, QString("Problemas durante o download de " + moduloName));
                return false;
            }
        } else {
            return true;
        }
    } else {
        log->escrever(LogCacic::ErrorLevel, QString("Não foi possível recuperar json de " +
                                                         this->cacicFolder + "/getConfig.json ao tentar baixar " +
                                                         moduloName));
        return false;
    }
}

