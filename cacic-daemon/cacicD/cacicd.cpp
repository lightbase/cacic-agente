#include "cacicd.h"

cacicD::cacicD(int argc, char **argv) : QtService<QCoreApplication>(argc, argv, "CacicDaemon")
{
    ccacic = new CCacic();
    ccacic->salvarVersao("cacic-service");
    QString folder = ccacic->getValueFromRegistry("Lightbase", "Cacic", "mainFolder").toString();
    ccacic->setCacicMainFolder(!folder.isEmpty() && !folder.isNull() ? folder : Identificadores::ENDERECO_PATCH_CACIC);
    this->createApplication(argc, argv);
    setServiceFlags(QtService::Default);
    logcacic = new LogCacic(Identificadores::LOG_DAEMON, ccacic->getCacicMainFolder()+"/Logs");
}

cacicD::~cacicD()
{
    try{
        logcacic->~LogCacic();
        application()->exit();
    } catch (...){
        logcacic->escrever(LogCacic::ErrorLevel, QString("Erro desconhecido no destrutor."));
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

        logcacic->escrever(LogCacic::InfoLevel, QString("Cacic " + Identificadores::AGENTE_VERSAO + " iniciado em " + ccacic->getCacicMainFolder() + "."));
        Ocacictimer->iniciarTimer();
    }catch (...){
        logcacic->escrever(LogCacic::ErrorLevel, QString("Erro desconhecido ao iniciar o serviço."));
    }
}

void cacicD::pause()
{
    try{
        logcacic->escrever(LogCacic::InfoLevel, QString("Serviço pausado."));
    } catch (...){
        logcacic->escrever(LogCacic::ErrorLevel, QString("Erro desconhecido ao pausar o serviço."));
    }
}

void cacicD::resume()
{
    try{
        logcacic->escrever(LogCacic::InfoLevel, QString("Serviço resumido."));
    } catch (...){
        logcacic->escrever(LogCacic::ErrorLevel, QString("Erro desconhecido ao resumir o serviço."));
    }
}

void cacicD::stop()
{
    try{
        logcacic->escrever(LogCacic::InfoLevel, QString("Serviço parado."));
        this->application()->quit();
    } catch (...){
        logcacic->escrever(LogCacic::ErrorLevel, QString("Erro desconhecido ao parar o serviço."));
    }
}
