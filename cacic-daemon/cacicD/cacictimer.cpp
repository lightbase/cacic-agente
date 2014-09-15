#include "cacictimer.h"

CacicTimer::CacicTimer()
{
    OCacicComm = new CacicComm();
    ccacic = new CCacic();
    timer = new QTimer(this);
    logManager = QLogger::QLoggerManager::getInstance();

    OCacicComm = CacicComm("http://teste.cacic.cc",
                                 "rG/HcIDVTZ3pPKCf[[MAIS]]I6aigUb7OMeij3FfC7qNaznk0rBRYb6q6kSK3eNfjgptS8BfwW5yJqCvD2ai7xlw9I6P21j6cvQUqlHmAJpCbfwR13urdRE9AhjfokMcPrH6R1/zXPGMHveLRRgKLcqWu2X96rmoQQdRq9EW1SXmYUAx1dCV[[MAIS]]3Ha61XBw5pq58q35zH8Gjt998rTi3ecV8ShXXevqyK[[MAIS]]W07xpgTjbbd6Fbs/35gPfdRRgMNFjq7Gq[[MAIS]]llFgYMJ6UcEhw8f0ZLQo2oL/eRW/CNyeBW6wG0hIo6EIdpi/Ht0/95Uwn2Og[[MAIS]]2UPXsmKKuTMeGwUvPdbEfexlinUO0[[MAIS]]j9qIa2dpjtl0Y5Fyk1Bvw2ZYRTXwgJpUHsBboWmtTFpgX3wSGOWMipE80K8ktRTVYOp[[MAIS]]4qS/SzKWXpfCuZoCncfwE0lCEoreTH[[MAIS]]MLrTkHJP2oqYMAyFyQcjC0UGr3BQGa2edSNXjG7jrTdddga/SODUiF94jgh/QBwhiZby34b__CRYPTED__",
                                 "P198PVwtz5F5CfZPSUrzuaQA/QG1sTnwzl/rBnj8M7y5MglANGodG5LLD4q7oY809HuDR4g5tL64lZRBKvKPmEgWd9iAZKvT4UAm9XWN3nKKLGaznCaJohmntNGqrJP1Zd9riTHGu10mPbg/Uh3TCbBHVOICvu5sDlINlCR6A3[[MAIS]]a55RhfKNidvr5uX0kozCxr5t2DyOb5oPocEGyJKyHLQ==__CRYPTED__",
                                 "1",
                                 "0",
                                 "0",
                                 "YwpgjzZ86/eCsjvOki1KkQ==__CRYPTED__",
                                 "8PT6U445MPNr3S7WHFa20ge/8NJSspSYE/UL3zf[[MAIS]]wkEfNZ7p1B/2o6i89gTZ44Ok__CRYPTED__",
                                 "wshPlZWvLPhbUEQFQ1aQmA==__CRYPTED__",
                                 "E9aKtVz/OSmGv7v1xW4FwaFukOob3qQ/HFDIz916K3ys82qfX9vBDz[[MAIS]]M2cdoYedhNkXt[[MAIS]]UVb10nD[[MAIS]]tOfbn7mlOlTDoGzUl9CgdPCsb[[MAIS]]Qa4wzuCrMw0BcCuaKfr62E3b8vh6Ug4JJbOgBo3ohG3x5O7Ap0D94GKtAWjMuTXRBbhGxPrRS4M6AhzvaN5P8pWd9FikrCLWuMs4ebnRlftef3RmVKScBDQ5zmK6sKxYNmX[[MAIS]]kKn/Gi2[[MAIS]]HftZ7HD9JLvZBpYhp4yjqShblQ67d6PlWgrRWXxwDseIiJe3NlF70zrCPBBelkeAhVDXMXnsYtLms3ElsLzxa3c5vpWzTfgyE[[MAIS]]hvALy4ivew7zaxEwrg2KY1T7GOSWZ7vljIvbhVZJgtzbnYiulFjJTtm5W4uJMKLuUJNzB0rxMkzToIE9/kDmeO2OKiwFbIfbe2Wdq0VpllOkjKtj9WM676iBSpYAXG0MsmLMj9CZ/sNi06hGF[[MAIS]]rXxiOSk66M0Hq[[MAIS]]OeUBh4z3Ra6f9yPJn0K9hIQyT[[MAIS]]Dr/QW/QdJIOkLTW8ZyyAF4DW9ojiTlUdySvB5EB1jyZ5nNTpzyE38qIdg7uM1tdWdm9FZGYxMdHwi1tS2dBeIWkDfWEjih01xiVt2dmdRgo0zIZvmmYDJ9LhCU3XPDlwdeQ8[[MAIS]]F7fnxDfxGqZQjZJ[[MAIS]]Y/Q8bdLpgbhcd3gt3SzjMLE0M[[MAIS]]hTmHG1KP2CzCibZ7grFAN3Y3jSPjmEj5Fzs26tdhdjNzSIt1FCueea8P2c4NmxPYkJCg0D1mgNrkH3L5svsO[[MAIS]]6YYrl3df4s[[MAIS]]fcK/to9s/pnkKfBKrAPF6Z4s2WPsALUPBDS5ty7RC2vqXRvH6hPR4o7xhXzQOQ/1xytr/RoEWEVeZ2wgHVVbbtFsALul/IDc7B45wmwxBx9iZHVwpQJVlNZZUBtwKqKhnXz5pM[[MAIS]]7yUAkfhMcO09pPd5kB2MBdRJs[[MAIS]]2FIk0GRUw05IHkc1rZEX5Ow==__CRYPTED__",
                                 "Yot8BeM9lOh431SB7dYQXw==__CRYPTED__",
                                 "Dcr8b5IfZOJjt6qyH5dGyw==__CRYPTED__",
                                 "2.5.1.1.256.32",
                                 "2.8.1.7",
                                 "2.8.1.6");
    OCacicComm->setUrlSsl("https://10.1.0.137/cacic/web/app_dev.php");
    OCacicComm->setUsuario("cacic");
    OCacicComm->setPassword("cacic123");

    connect(timer,SIGNAL(timeout()),this,SLOT(mslot()));
}

CacicTimer::~CacicTimer()
{
    logManager->closeLogger();
    delete logManager;
    delete ccacic;
    delete OCacicComm;
}

void CacicTimer::iniciarTimer(int x, QString applicationDirPath)
{
    setApplicationDirPath(applicationDirPath);
    logManager->addDestination(getApplicationDirPath().append("/cacicLog.txt"), QStringList("Cacic Daemon"), QLogger::InfoLevel);
    logManager->addDestination(getApplicationDirPath().append("/cacicLog.txt"), QStringList("Cacic Daemon"), QLogger::ErrorLevel);
    timer->start(x);
}

void CacicTimer::mslot(){
    if(getTest()){
        //manager->QLogger::QLog_Trace("ModuleName", "Message: ");
        if(getConfig()){
            qDebug() << "getConfig() success. - " + QDateTime::currentDateTime().toLocalTime().toString();
            QLogger::QLog_Info("Cacic Daemon", "getConfig() executado com sucesso.");

            // Compara o novo arquivo de configuração com um antigo e se forem diferentes
            // mantem o mais recente; caso iguais simplesmente apaga o novo.
            QFile *fileOld;
            QFile *fileNew;

            fileOld = new QFile(getApplicationDirPath().append("/getConfig.json"));
            fileNew = new QFile(getApplicationDirPath().append("/getConfigNew.json"));
            if( fileOld->exists() && fileNew->exists() ){
                if( Md5IsEqual(QVariant::fromValue(fileOld), QVariant::fromValue(fileNew)) ) {
                    fileNew->remove();
                } else {
                    // Renomeia getConfigNew.json para getConfig.json
                    fileOld->remove();
                    fileNew->rename("getConfigNew.json","getConfig.json");
                }
                jsonConfig = ccacic->getJsonFromFile(getApplicationDirPath().append("/getConfig.json"));
            } else if( fileOld->exists() ){
                jsonConfig = ccacic->getJsonFromFile(getApplicationDirPath().append("/getConfig.json"));
            } else {
                QLogger::QLog_Error("Cacic Daemon", "Arquivo de configuração não criado.");
            }
            delete fileOld;
            delete fileNew;

            /* lê json de configurações e armazena quais módulos executáveis.
             * E faz o mesmo tipo de comparação de hashs, com o fim de:
             * ou mantem o binário do módulo ou baixa um novo.
             */

            /* Aqui estou assumindo um formato do .json em que:
             * há a key modulos contem uma lista com o nome dos executaveis e os seus valores hash md5
             * há a key metodo que explicita o método de download dos executaveis
             */
            foreach( QJsonValue moduloValues, jsonConfig["modulos"].toArray() ) {
                QString moduloKey, moduloValue;

                moduloKey = moduloValues.toObject()["md5"].toString();
                moduloValue = moduloValues.toObject()["nome"].toString();

                moduleMap.insert(moduloKey, moduloValue);
            }
            metodoDownload = jsonConfig["metodo"].toString();

            int countExecNotFound = 0;
            QMap<QString, QString>::const_iterator mapIterator = moduleMap.constBegin();
            while (mapIterator != moduleMap.constEnd()) {
                QString nomeModulo = mapIterator.value();
                QString hashModulo = mapIterator.key();

                // Calcula hash do binario atual
#if defined(Q_OS_WIN)
                fileOld = new QFile(getApplicationDirPath().append("/").append(modulo).append(".exe"));
#else
                fileOld = new QFile(getApplicationDirPath().append("/").append(nomeModulo));
#endif
                if(!fileOld->exists()) {
                    QLogger::QLog_Error("Cacic Daemon", QString("Módulo ").append(nomeModulo).append(" não encontrado."));

                    countExecNotFound++;
                    if( countExecNotFound == moduleMap.size() ) {
                        QLogger::QLog_Error("Cacic Daemon", "Não foi possível encontrar nenhum módulo executável!");
                        return;
                    }
                    continue;
                }

                QString oldMd5 = QString(QCryptographicHash::hash(fileOld->readAll(),QCryptographicHash::Md5).toHex());

                if ( oldMd5 != hashModulo ) {

                    fileOld->rename(getApplicationDirPath().append("/").append(nomeModulo),
                                    getApplicationDirPath().append("/").append("nomeModulo").append("Old") );

                    // Download nova versão do executável
                    if(metodoDownload == "ftp" || metodoDownload == "") {
//                        OCacicComm->ftpDownload( QString("/ws/get/").append(nomeModulo) );
                    }


                    fileOld->remove();
                    delete fileOld;
                }

                mapIterator++;
            }

            //aqui irá abrir o semaforo e iniciar a coleta.

        }else{
            qDebug() << "getConfig() failed. - " + QDateTime::currentDateTime().toLocalTime().toString();
            QLogger::QLog_Error("Cacic Daemon", "Falha na obtenção do arquivo de configuração.");
        }
    }else{
        qDebug() << "getTest() failed. - " + QDateTime::currentDateTime().toLocalTime().toString();
        QLogger::QLog_Error("Cacic Daemon", "Falha na execução do getTest().");
    }
}

bool CacicTimer::getTest(){
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
            ccacic->setJsonToFile(jsonresult, getApplicationDirPath() + "/getTest.json");
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

bool CacicTimer::getConfig(){
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
            ccacic->setJsonToFile(jsonresult, getApplicationDirPath().append("/getConfigNew.json"));
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

void CacicTimer::registraInicio()
{
}

QString CacicTimer::getApplicationDirPath() const
{
    return applicationDirPath;
}

void CacicTimer::setApplicationDirPath(const QString &value)
{
    applicationDirPath = value;
}


void CacicTimer::registraFim()
{
}

bool CacicTimer::Md5IsEqual(QVariant document01,QVariant document02){
    QString getconfigMD5 = QString(QCryptographicHash::hash(
                                       (document01.toByteArray()),QCryptographicHash::Md5).toHex());
    QString getconfigMD52 = QString(QCryptographicHash::hash(
                                        (document02.toByteArray()),QCryptographicHash::Md5).toHex());
    if(getconfigMD5 == getconfigMD52){
        return true;
    }else{
        return false;
    }
}
