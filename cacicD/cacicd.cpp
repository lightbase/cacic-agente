#include "cacicd.h"

cacicD::cacicD(int argc, char **argv) : QtService<QCoreApplication>(argc, argv, "CacicD")
{
    try{
        setServiceDescription("Cacic Daemon");
        setServiceFlags(QtServiceBase::CannotBeStopped);
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

void cacicD::start()
{
    try{
        QCoreApplication *app = application();
        qDebug() << "Serviço iniciado.";
        qDebug() << app->applicationDirPath();

    } catch (...){
        qCritical() << "Error desconhecido no desconstrutor.";
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
