#include "cacicd.h"

cacicD::cacicD(int argc, char **argv) : QtService<QCoreApplication>(argc, argv, "CacicDaemon")
{
    QString folder = CCacic::getValueFromRegistry("Lightbase", "Cacic", "mainFolder").toString();
    cacicMainFolder = !folder.isEmpty() && !folder.isNull() ? folder : Identificadores::ENDERECO_PATCH_CACIC;
    logcacic = new LogCacic(LOG_DAEMON, cacicMainFolder + "/Logs");
    this->createApplication(argc, argv);
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
//        SocketListener *socket;
//        socket = new SocketListener(cacicMainFolder);
//        QObject::connect(socket, SIGNAL(forcaColeta()), Ocacictimer, SLOT(iniciarThread()));
        CacicTimer *Ocacictimer;
        Ocacictimer = new CacicTimer(cacicMainFolder);
        QObject::connect(Ocacictimer, SIGNAL(finalizar()), this->application(), SLOT(quit()));

        logcacic->escrever(LogCacic::InfoLevel, QString("Cacic " + Identificadores::AGENTE_VERSAO + " iniciado em " + cacicMainFolder + "."));
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
