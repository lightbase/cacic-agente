#include "cacicd.h"

cacicD::cacicD(int argc, char **argv) : QtService<QCoreApplication>(argc, argv, "CacicDaemon")
{
    try{
        this->createApplication(argc, argv);

        Ocacictimer = new CacicTimer(application()->applicationDirPath());
        ccacic = new CCacic;
        setServiceDescription("Cacic Daemon");
        setServiceFlags(QtService::Default);

        logManager = QLogger::QLoggerManager::getInstance();
        logManager->addDestination(application()->applicationDirPath() + "/cacicLog.txt","Cacic Daemon",QLogger::InfoLevel);
        logManager->addDestination(application()->applicationDirPath() + "/cacicLog.txt","Cacic Daemon",QLogger::ErrorLevel);
    } catch (...){
        qCritical() << "Error desconhecido no construtor.";
    }
}

cacicD::~cacicD()
{
    try{
        logManager->closeLogger();
        application()->exit();
    } catch (...){
        qCritical() << "Error desconhecido no desconstrutor.";
    }
}

void cacicD::start() {
    try{
        qDebug() << "Inicio do daemon";
        qDebug() << "\nServiço iniciado em" << application()->applicationDirPath();
        QLogger::QLog_Info("Cacic Daemon", QString("Servico iniciado em ").append(application()->applicationDirPath()).append("."));
        QJsonObject result = ccacic->getJsonFromFile(application()->applicationDirPath().append("/getConfig.json"));
        if(!result.contains("error") && !result.isEmpty()){
            Ocacictimer->iniciarTimer(result["codestatus"].toInt());
        }else{
            qDebug() << "getConfig.json não encontrado.";
        }
        //QLogger::QLog_Info("Cacic Daemon", "Serviço finalizado.");
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


