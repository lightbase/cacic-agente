#include "cacicd.h"

cacicD::cacicD(int argc, char **argv) : QtService<QCoreApplication>(argc, argv, "CacicDaemon")
{
    ccacic = new CCacic();
    ccacic->salvarVersao("cacic-service");
    QString folder = ccacic->getValueFromRegistry("Lightbase", "Cacic", "mainFolder").toString();
    ccacic->setCacicMainFolder(!folder.isEmpty() && !folder.isNull() ? folder : Identificadores::ENDERECO_PATCH_CACIC);
    logManager = QLogger::QLoggerManager::getInstance();
    logManager->addDestination(ccacic->getCacicMainFolder() + "/Logs/cacic.log", Identificadores::LOG_DAEMON, QLogger::InfoLevel);
    logManager->addDestination(ccacic->getCacicMainFolder() + "/Logs/cacic.log",Identificadores::LOG_DAEMON ,QLogger::ErrorLevel);
    this->createApplication(argc, argv);
    setServiceFlags(QtService::Default);
}

cacicD::~cacicD()
{
    try{
        logManager->closeLogger();
        application()->exit();
    } catch (...){
        QLogger::QLog_Error(Identificadores::LOG_DAEMON, QString("Erro desconhecido no destrutor."));
    }
}

void cacicD::start() {
    try{
        CacicTimer *Ocacictimer;
        SocketListener *socket;

        Ocacictimer = new CacicTimer(ccacic->getCacicMainFolder());
        socket = new SocketListener(ccacic->getCacicMainFolder());
        QObject::connect(Ocacictimer, SIGNAL(finalizar()), this->application(), SLOT(quit()));
        QObject::connect(socket, SIGNAL(forcaColeta()), Ocacictimer, SLOT(iniciarThread()));

        QLogger::QLog_Info(Identificadores::LOG_DAEMON, QString("Cacic " + Identificadores::AGENTE_VERSAO + " iniciado em " + ccacic->getCacicMainFolder() + "."));
        Ocacictimer->iniciarTimer();
    }catch (...){
        QLogger::QLog_Info(Identificadores::LOG_DAEMON, QString("Erro desconhecido ao iniciar o serviço."));
    }
}

void cacicD::pause()
{
    try{
        QLogger::QLog_Info(Identificadores::LOG_DAEMON, QString("Serviço pausado."));
    } catch (...){
        QLogger::QLog_Error(Identificadores::LOG_DAEMON, QString("Erro desconhecido ao pausar o serviço."));
    }
}

void cacicD::resume()
{
    try{
        QLogger::QLog_Info(Identificadores::LOG_DAEMON, QString("Serviço resumido."));
    } catch (...){
        QLogger::QLog_Error(Identificadores::LOG_DAEMON, QString("Erro desconhecido ao resumir o serviço."));
    }
}

void cacicD::stop()
{
    try{
        QLogger::QLog_Info(Identificadores::LOG_DAEMON, QString("Serviço parado."));
        this->application()->quit();
    } catch (...){
        QLogger::QLog_Error(Identificadores::LOG_DAEMON, QString("Erro desconhecido ao parar o serviço."));
    }
}

