#include "cacicdeploy.h"

cacicdeploy::cacicdeploy(int argc, char **argv) : QtService<QCoreApplication>(argc, argv, "CacicDeploy")
{
    ccacic = new CCacic();
    QString folder = ccacic->getValueFromRegistry("Lightbase", "Cacic", "mainFolder").toString();
    ccacic->setCacicMainFolder(!folder.isEmpty() && !folder.isNull() ? folder : Identificadores::ENDERECO_PATCH_CACIC);
    logcacic = new LogCacic(LOG_CACICDEPLOY, ccacic->getCacicMainFolder()+"/Logs");
    this->createApplication(argc, argv);
}

cacicdeploy::~cacicdeploy()
{
    try{
        logcacic->~LogCacic();
        application()->exit();
    } catch (...){
        logcacic->escrever(LogCacic::ErrorLevel, QString("Erro desconhecido no destrutor."));
    }
}

void cacicdeploy::start() {
    try{
        timer = new deployTimer(ccacic);
        timer->start(10000, 5000);
    }catch (...){
        logcacic->escrever(LogCacic::ErrorLevel, QString("Erro desconhecido ao iniciar o serviço."));
    }
}

void cacicdeploy::pause()
{
    try{
        logcacic->escrever(LogCacic::InfoLevel, QString("Serviço pausado."));
    } catch (...){
        logcacic->escrever(LogCacic::ErrorLevel, QString("Erro desconhecido ao pausar o serviço."));
    }
}

void cacicdeploy::resume()
{
    try{
        logcacic->escrever(LogCacic::InfoLevel, QString("Serviço resumido."));
    } catch (...){
        logcacic->escrever(LogCacic::ErrorLevel, QString("Erro desconhecido ao resumir o serviço."));
    }
}

void cacicdeploy::stop()
{
    try{
        logcacic->escrever(LogCacic::InfoLevel, QString("Serviço parado."));
        this->application()->quit();
    } catch (...){
        logcacic->escrever(LogCacic::ErrorLevel, QString("Erro desconhecido ao parar o serviço."));
    }
}
