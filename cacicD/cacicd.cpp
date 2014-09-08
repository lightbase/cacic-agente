#include "cacicd.h"

cacicD::cacicD(int argc, char **argv) : QtService<QCoreApplication>(argc, argv, "CacicD")
{
    try{
        OCacicComm = new CacicComm();
        OCacicTimer = new CacicTimer();
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
        if(!result.contains("error")){
            OCacicTimer->iniciarTimer(result["codestatus"].toInt());
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

bool cacicD::getTest(){
    try{
        qDebug() << app->applicationDirPath() + "getTest.json";
        OCacicComm->setUrlGerente("http://10.1.0.137/cacic/web/app_dev.php");
        OCacicComm->setUsuario("cacic");
        OCacicComm->setPassword("cacic123");
        bool ok;
        QJsonObject as;
        as["computador"] = OCacic_Computer.toJsonObject();
        QJsonObject jsonresult = OCacicComm->comm("/ws/neo/login", &ok, as);
        if(jsonresult.contains("error")){
            return false;
        }
        try{
            qDebug() << app->applicationDirPath() + "getTest.json";
            ccacic->setJsonToFile(jsonresult, app->applicationDirPath() + "/getTest.json");
            return true;
        } catch (...) {
            qDebug() << "Erro ao salvar o arquivo de configurações.";
            return false;
        }
    } catch (...){
        qDebug() << "Erro ao conectar com o servidor.";
        return false;
    }
}

bool cacicD::getConfig(){
    try{
        OCacicComm->setUrlGerente("http://10.1.0.137/cacic/web/app_dev.php");
        OCacicComm->setUsuario("cacic");
        OCacicComm->setPassword("cacic123");
        bool ok;
        QJsonObject as;
        as["computador"] = OCacic_Computer.toJsonObject();
        QJsonObject jsonresult = OCacicComm->comm("/ws/neo/login", &ok, as);
        if(jsonresult.contains("error")){
            return false;
        }
        try{
            ccacic->setJsonToFile(jsonresult, app->applicationDirPath() + "/getConfig.json");
            return true;
        } catch (...) {
            qDebug() << "Erro ao salvar o arquivo de configurações.";
            return false;
        }
    } catch (...){
        qDebug() << "Erro ao conectar com o servidor.";
        return false;
    }
}

void cacicD::registraInicio()
{

}

void cacicD::registraFim()
{

}

bool cacicD::compararHashMD5(QJsonDocument getconfigfile,QJsonDocument getConfig){
    QString getconfigMD5 = QString(QCryptographicHash::hash(
                                       (getconfigfile.toVariant().toByteArray()),QCryptographicHash::Md5).toHex());
    QString getconfigMD52 = QString(QCryptographicHash::hash(
                                        (getConfig.toVariant().toByteArray()),QCryptographicHash::Md5).toHex());
    if(getconfigMD5 == getconfigMD52){
        return true;
    }else{
        return false;
    }
}
