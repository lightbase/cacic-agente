#include "cacicd.h"

cacicD::cacicD(int argc, char **argv) : QtService<QCoreApplication>(argc, argv, "CacicD")
{
    try{
        Ocacictimer = new CacicTimer();
        ccacic = new CCacic;
        setServiceDescription("Cacic Daemon");
        setServiceFlags(QtService::Default);
    } catch (...){
        qCritical() << "Error desconhecido no construtor.";
    }
}

cacicD::~cacicD()
{
    try{

    } catch (...){
        qCritical() << "Error desconhecido no desconstrutor.";
    }
}

void cacicD::start() {
    try{
        this->app = application();
        qDebug() << "\nServiço iniciado em" << app->applicationDirPath();
        QJsonObject result = ccacic->getJsonFromFile(app->applicationDirPath() + "/getConfig.json");
        if(!result.contains("error") && !result.isEmpty()){
            Ocacictimer->iniciarTimer(result["codestatus"].toInt(), app->applicationDirPath());
        }else{
            qDebug() << "getConfig.json não encontrado.";
        }
    }catch (...){
        qCritical() << "Error desconhecido ao iniciar o serviço.";
    }
}

void cacicD::pause()
{
    try{
        qDebug() << "Serviço pausado.";
    } catch (...){
        qCritical() << "Error desconhecido ao pausar o serviço.";
    }
}

void cacicD::resume()
{
    try{
        qDebug() << "Serviço resumido.";
    } catch (...){
        qCritical() << "Error desconhecido ao resumir o serviço.";
    }
}

void cacicD::stop()
{
    try{
        qDebug() << "Serviço parado.";
    } catch (...){
        qCritical() << "Error desconhecido ao parar o serviço.";
    }
}


