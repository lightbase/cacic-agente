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
        QLogger::QLog_Error("Cacic Daemon", QString("Erro desconhecido no construtor."));
    }
}

cacicD::~cacicD()
{
    try{
        logManager->closeLogger();
        application()->exit();
    } catch (...){
        QLogger::QLog_Error("Cacic Daemon", QString("Erro desconhecido no destrutor."));
    }
}

void cacicD::start() {
    try{
        QLogger::QLog_Info("Cacic Daemon", QString("Servico iniciado em ").append(application()->applicationDirPath()).append("."));
        QJsonObject result = ccacic->getJsonFromFile(application()->applicationDirPath().append("/getConfig.json"));
        if(!result.contains("error") && !result.isEmpty()){
            Ocacictimer->setPeriodicidadeExecucao(result["codestatus"].toInt());
            Ocacictimer->iniciarTimer();
        }else{
            //TODO: deve iniciar com um timer default (4 horas é um tempo bom), pra não ficar freezado pra sempre.
            QLogger::QLog_Error("Cacic Daemon", QString("getConfig.json não encontrado."));
        }
        //QLogger::QLog_Info("Cacic Daemon", "Serviço finalizado.");
    }catch (...){
        QLogger::QLog_Error("Cacic Daemon", QString("Erro desconhecido ao iniciar o serviço."));
    }
}

void cacicD::pause()
{
    try{
        QLogger::QLog_Info("Cacic Daemon", QString("Serviço pausado."));
    } catch (...){
        QLogger::QLog_Error("Cacic Daemon", QString("Erro desconhecido ao pausar o serviço."));
    }
}

void cacicD::resume()
{
    try{
        QLogger::QLog_Info("Cacic Daemon", QString("Serviço resumido."));
    } catch (...){
        QLogger::QLog_Error("Cacic Daemon", QString("Erro desconhecido ao resumir o serviço."));
    }
}

void cacicD::stop()
{
    try{
        QLogger::QLog_Info("Cacic Daemon", QString("Serviço parado."));
        this->application()->quit();
    } catch (...){
        QLogger::QLog_Error("Cacic Daemon", QString("Erro desconhecido ao parar o serviço."));
    }
}


