#include "chksys.h"

chksys::chksys(int argc, char **argv) : QtService<QCoreApplication>(argc, argv, "Check Cacic")
{
    this->cacicFolder = CCacic::getValueFromRegistry("Lightbase", "Cacic", "mainFolder").toString();
    if (this->cacicFolder.isEmpty() || this->cacicFolder.isNull())
        this->cacicFolder = Identificadores::ENDERECO_PATCH_CACIC;
    if (!this->cacicFolder.endsWith("/")) this->cacicFolder.append("/");
    logcacic = new LogCacic(LOG_CHKSYS, this->cacicFolder+"/Logs");
    this->createApplication(argc, argv);
}

chksys::~chksys()
{
    try{
        logcacic->~LogCacic();
        application()->exit();
    } catch (...){
        logcacic->escrever(LogCacic::ErrorLevel, QString("Erro desconhecido no destrutor."));
    }
}

void chksys::start() {
    try{
        timer = new chksysTimer(this->cacicFolder);
        timer->start(60000);
    }catch (...){
        logcacic->escrever(LogCacic::ErrorLevel, QString("Erro desconhecido ao iniciar o serviço."));
    }
}

void chksys::pause()
{
    try{
        logcacic->escrever(LogCacic::InfoLevel, QString("Serviço pausado."));
    } catch (...){
        logcacic->escrever(LogCacic::ErrorLevel, QString("Erro desconhecido ao pausar o serviço."));
    }
}

void chksys::resume()
{
    try{
        logcacic->escrever(LogCacic::InfoLevel, QString("Serviço resumido."));
    } catch (...){
        logcacic->escrever(LogCacic::ErrorLevel, QString("Erro desconhecido ao resumir o serviço."));
    }
}

void chksys::stop()
{
    try{
        logcacic->escrever(LogCacic::InfoLevel, QString("Serviço parado."));
        this->application()->quit();
    } catch (...){
        logcacic->escrever(LogCacic::ErrorLevel, QString("Erro desconhecido ao parar o serviço."));
    }
}
