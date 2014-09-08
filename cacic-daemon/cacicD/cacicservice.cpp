#include "cacicd.h"

cacicD::cacicD(int argc, char **argv) : QtService<QCoreApplication>(argc, argv, "CacicD")
{
    try{
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

void cacicD::start()
{
    try{
        this->app = application();
        qDebug() << "\nServiço iniciado em" << app->applicationDirPath();
        if(getTest()){
            qDebug() << "\ngetTest() success.";
        }else{
            qDebug() << "\ngetTest() error.";
        }
        if(getConfig()){
            qDebug() << "\ngetconfig() success.";
        }else{
            qDebug() << "\ngetconfig() error.";
        }
    } catch (...){
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
        CacicComm  OCacicComm;
        OCacicComm.setUrlGerente("http://10.1.0.137/cacic/web/app_dev.php");
        OCacicComm.setUsuario("cacic");
        OCacicComm.setPassword("cacic123");
        bool ok;
        QJsonObject as;
        as["computador"] = OCacic_Computer.toJsonObject();
        QJsonObject jsonresult = OCacicComm.comm("/ws/neo/login", &ok, as);
        try{
            saveJson(QJsonDocument::fromVariant(jsonresult.toVariantMap()), "getTest.conf");
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
        CacicComm  OCacicComm;
        OCacicComm.setUrlGerente("http://10.1.0.137/cacic/web/app_dev.php");
        OCacicComm.setUsuario("cacic");
        OCacicComm.setPassword("cacic123");
        bool ok;
        QJsonObject as;
        as["computador"] = OCacic_Computer.toJsonObject();
        QJsonObject jsonresult = OCacicComm.comm("/ws/neo/login", &ok, as);
        try{
            saveJson(QJsonDocument::fromVariant(jsonresult.toVariantMap()), "getConfig.conf");
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

void cacicD::saveJson(QJsonDocument document, QString fileName) {
#if defined(Q_OS_LINUX)
    QFile jsonFile(app->applicationDirPath().append("/" + fileName));
#elif defined(Q_OS_WIN)
    QFile jsonFile(app->applicationDirPath().append("\\" + fileName));
#endif
    jsonFile.open(QFile::WriteOnly);
    jsonFile.write(document.toJson());
    jsonFile.close();
}

QJsonDocument cacicD::loadJson(QString fileName) {
#if defined(Q_OS_LINUX)
    QFile jsonFile(app->applicationDirPath().append("/" + fileName));
#elif defined(Q_OS_WIN)
    QFile jsonFile(app->applicationDirPath().append("\\" + fileName));
#endif
    jsonFile.open(QFile::ReadOnly);
    return QJsonDocument().fromJson(jsonFile.readAll());
}
