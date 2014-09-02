#include "cacicdaemon.h"

CacicDaemon::CacicDaemon(QObject * parent):
    QThread(parent)
{
    abort = false;
    paused = false;
}

CacicDaemon::~CacicDaemon()
{
    mutex.lock();
    abort = true;
    condition.wakeOne();
    mutex.unlock();

    wait();
}

void CacicDaemon::run()
{
    while(!paused && !abort) {
        sleep(5000);
        getTest();
        sleep(1000);
        getConfig();

        if(abort)
            return;
    }
}

void CacicDaemon::pause()
{
    mutex.lock();
    paused = true;
    mutex.unlock();
}

void CacicDaemon::resume()
{
    mutex.lock();
    paused = false;
    condition.wakeOne();
    mutex.unlock();
}

bool CacicDaemon::getConfig()
{
    qDebug() << ">>> getConfig()";
    return true;

//    try{
//        CacicComm  OCacicComm;
//        OCacicComm.setUrlGerente("http://10.1.0.137/cacic/web/app_dev.php");
//        OCacicComm.setUsuario("cacic");
//        OCacicComm.setPassword("cacic123");
//        bool ok;
//        QJsonObject as;
//        as["computador"] = OCacic_Computer.toJsonObject();
//        QJsonObject jsonresult = OCacicComm.comm("/ws/neo/login", &ok, as);
//        try{
//            saveJson(QJsonDocument::fromVariant(jsonresult.toVariantMap()), "getConfig.conf");
//            return true;
//        } catch (...) {
//            qDebug() << "Erro ao salvar o arquivo de configurações.";
//            return false;
//        }
//    } catch (...){
//        qDebug() << "Erro ao conectar com o servidor.";
//        return false;
//    }
}

bool CacicDaemon::getTest()
{
    qDebug() << ">>> getTest()";
    return true;

//    try{
//        CacicComm  OCacicComm;
//        OCacicComm.setUrlGerente("http://10.1.0.137/cacic/web/app_dev.php");
//        OCacicComm.setUsuario("cacic");
//        OCacicComm.setPassword("cacic123");
//        bool ok;
//        QJsonObject as;
//        as["computador"] = OCacic_Computer.toJsonObject();
//        QJsonObject jsonresult = OCacicComm.comm("/ws/neo/login", &ok, as);
//        try{
//            saveJson(QJsonDocument::fromVariant(jsonresult.toVariantMap()), "getTest.conf");
//            return true;
//        } catch (...) {
//            qDebug() << "Erro ao salvar o arquivo de configurações.";
//            return false;
//        }
//    } catch (...){
//        qDebug() << "Erro ao conectar com o servidor.";
//        return false;
//    }
}


//void cacicD::saveJson(QJsonDocument document, QString fileName) {
//#if defined(Q_OS_LINUX)
//    QFile jsonFile(app->applicationDirPath().append("/" + fileName));
//#elif defined(Q_OS_WIN)
//    QFile jsonFile(app->applicationDirPath().append("\\" + fileName));
//#endif
//    jsonFile.open(QFile::WriteOnly);
//    jsonFile.write(document.toJson());
//    jsonFile.close();
//}

//QJsonDocument cacicD::loadJson(QString fileName) {
//#if defined(Q_OS_LINUX)
//    QFile jsonFile(app->applicationDirPath().append("/" + fileName));
//#elif defined(Q_OS_WIN)
//    QFile jsonFile(app->applicationDirPath().append("\\" + fileName));
//#endif
//    jsonFile.open(QFile::ReadOnly);
//    return QJsonDocument().fromJson(jsonFile.readAll());
//}

