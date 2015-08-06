#include "testcacic.h"

QTEST_MAIN(CTestCacic)

CTestCacic::CTestCacic(QObject *parent) :
    QObject(parent)
{
}

void CTestCacic::initTestCase()
{
    this->OCacicComm = new CacicComm("Cacic Teste", this->testPath);
    this->testPath = QDir::currentPath() + "/teste";
    this->testIniPath = testPath + "/teste.ini";

    OCacicComm->setUrlGerente("http://teste.cacic.cc");
    OCacicComm->setUsuario("cacic");
    OCacicComm->setPassword("cacic123");

    QVariantMap jsonMap;
    jsonMap["session"] = "lakdhfalkfhsaklfhasfhsl";
    this->session = QJsonObject::fromVariantMap(jsonMap);
}

void CTestCacic::testClassDestructors()
{
    //Caso o constructor de alguma classe dê pau, o teste vai dar crash aqui.
    for (int i=0;i<=7;i++){
        if (i==0){
            CacicComm *OCacicCommT;
            OCacicCommT = new CacicComm("teste", this->testPath);
        } else if (i==1){
            CACIC_Computer OCacicCompT;
        } else if (i==2){
            cacic_software OCacicSoftwareT;
        } else if (i==3){
            ConsoleObject console;
        } else if (i==4){
            cacic_hardware OCacicHardwareT;
        } else if (i==5){
            CColeta oColetaT;
        } else if (i==6){
            CheckModules *oCheckModulesT;
            oCheckModulesT = new CheckModules(this->testPath,"teste");
        } else if (i==7) {
            LogCacic *logcacic;
            logcacic = new LogCacic("qqq", this->testPath);
        }
    }
    QVERIFY(true);
}

void CTestCacic::testCreateFolder()
{
    QVERIFY(CCacic::createFolder(testPath));
}

//void CTestCacic::testGetAndSetValueFromFile()
//{
//    CCacic::setValueToFile("Teste", "teste", "Valor de teste", testIniPath);
//    QVERIFY( CCacic::getValueFromFile("Teste", "teste", testIniPath) == "Valor de teste");
//}

//void CTestCacic::testGetValueFromTags()
//{
//    QString value = "blablab[teste]Valor de teste[/teste]feihgj";
//    QVERIFY(CCacic::getValueFromTags(value, "teste") == "Valor de teste");
//}

void CTestCacic::testCommStatus()
{
    QVERIFY(OCacicComm->commStatus());
}

//void CTestCacic::testComm()
//{
//    bool ok;
//    if (OCacicComm->commStatus()){
//        QJsonObject jsonreply = OCacicComm->comm("/ws/neo", &ok);
//        //      qDebug() << jsonreply["codestatus"].toString();
//        QVERIFY(CCacic::getValueFromTags(jsonreply["reply"].toString(), "Comm_Status", "<>") == QString("OK"));
//    } else
//        QSKIP("Teste de comunicação negativo!");
//}

void CTestCacic::testDeleteFile()
{
    QDir file(testIniPath);
    CCacic::deleteFile(testIniPath);
    QVERIFY(!file.exists());
}

void CTestCacic::testProcFind()
{
#ifndef Q_OS_WIN
    QVERIFY(CCacic::findProc("init"));
#else
    QSKIP("Teste desnecessário nessa plataforma");
#endif
}

void CTestCacic::testDeleteFolder()
{
    QDir folder(testPath+"/testDeleteFolder/");
    CCacic::createFolder(folder.absolutePath());
    CCacic::deleteFolder(folder.absolutePath());
    QVERIFY(!folder.exists());
}

void CTestCacic::testInterfaceDeRede(){
    //    qDebug() << OCacicComp.getNetworkInterface().at(0)["nome"].toString();
    QVERIFY2(!OCacicComp.getNetworkInterface().empty(), "Nenhuma interface de rede encontrada.");
}

void CTestCacic::testcoletaIdOs(){
    // instancia um OperatingSystem que invoca coletaIdOs() no construtor
    OperatingSystem OS;

    //    QVERIFY(OS.getIdOs() == OperatingSystem::LINUX_ARCH);
    QVERIFY(OS.getIdOs() != -1);
}

void CTestCacic::testpegarOS(){
    //basta que seja um retorno válido, não precisa validar todos.
    QVERIFY(OCacicComp.getOs().getIdOs() != -1 );
}

void CTestCacic::testConsole()
{
#if defined(Q_OS_LINUX)
    ConsoleObject console;
    QVERIFY(console("echo teste").toStdString() == "teste\n");
#else
    QSKIP("Teste desnecessário nessa plataforma");
#endif
}

void CTestCacic::testPegarUsu(){

    OperatingSystem OS;

    if(OS.getIdOs() == OperatingSystem::LINUX_ARCH) // No Arch o comando who está retornando vazio
        QVERIFY(OCacicComp.getUser() == "");
    else
        QVERIFY(OCacicComp.getUser() != "");
}

//void CTestCacic::testJsonValueFromJsonString()
//{
//    QVERIFY(CCacic::jsonValueFromJsonString("{\"nome\":\"teste\"}", "nome").toString() == "teste");
//}

void CTestCacic::testLogin(){
    bool ok;
    QJsonObject jsonReply = OCacicComm->login(&ok);
    //  qDebug() << jsonReply;
    QJsonObject sessionvalue = jsonReply["reply"].toObject();
//    CCacic::setChaveCrypt(sessionvalue["chavecrip"].toString());
    QVERIFY(ok);
}

void CTestCacic::testSslConnection()
{
    bool ok;
    QJsonObject json = OCacicComm->comm("", &ok, QJsonObject(), true);
    QJsonValue jsonvalue = (!json["codestatus"].isNull()) ?
                json["codestatus"] :
        QJsonValue::fromVariant("-1");
    //    qDebug()<< jsonvalue.toDouble();
    QVERIFY(jsonvalue.toString() == "200" || jsonvalue.toString() == "302");
}

//void CTestCacic::testEnCrypt(){
//    std::string IV = "0123456789123456"; //iv nunca se repete para a mesma senha.
//    std::string input = "aqui vai a url que sera encriptada";
//    CCacic::setChaveCrypt("testecript123456");
//    this->cripTeste = CCacic::enCrypt(input, IV);
//    QVERIFY(!this->cripTeste.isEmpty() && !this->cripTeste.isNull());
//}

//void CTestCacic::testDeCrypt(){
//    std::string IV = "0123456789123456asas"; //iv nunca se repete para a mesma senha.
//    std::string input = this->cripTeste.toStdString();
//    QVERIFY(CCacic::deCrypt(input, IV) == "aqui vai a url que sera encriptada");

//}

void CTestCacic::testCacicCompToJsonObject()
{
    //    qDebug() << OCacicComp.toJsonObject();
    //    CCacic::setJsonToFile(OCacicComp.toJsonObject(), "jsoncomp.json");
    QVERIFY(!OCacicComp.toJsonObject().empty());
}

void CTestCacic::testOperatingSystemToJsonObject()
{
    QVERIFY(!OCacicComp.getOs().toJsonObject().empty());
}

void CTestCacic::testJsonToFile()
{
    QJsonObject json;
    json["teste"] = QJsonValue::fromVariant(QString("teste"));
    QVERIFY(CCacic::setJsonToFile(json, this->testPath + "/teste.json"));
}

void CTestCacic::testJsonFromFile()
{
    //    qDebug() << CCacic::getJsonFromFile("teste123.json");
    QVERIFY(CCacic::getJsonFromFile(this->testPath + "/teste.json")["teste"].toString() == "teste");
}

void CTestCacic::testSetRegistry()
{
    if( CCacic::verificarRoot() ) {
        QVariantMap valueMap;
        valueMap["teste1"] = QString("Teste 1");
        valueMap["teste2"] = QString("Teste2");
        CCacic::setValueToRegistry("Lightbase", "Teste", valueMap);
        QSettings confirmaTeste("Lightbase", "Teste");
        QCOMPARE(confirmaTeste.value("teste1").toString(), QString("Teste 1"));
    } else {
        QSKIP("Requer execução como root.");
    }

}

void CTestCacic::testGetValueFromRegistry()
{
    if( CCacic::verificarRoot() ) {
        QCOMPARE(CCacic::getValueFromRegistry("Lightbase", "Teste", "teste1"), QVariant("Teste 1"));
    } else {
        QSKIP("Requer execução como root.");
    }
}

void CTestCacic::testRemoveRegistry()
{
    CCacic::removeRegistry("Lightbase", "Teste");
    QSettings confirmaTeste("Lightbase", "Teste");
    QVERIFY(confirmaTeste.allKeys().isEmpty());
    confirmaTeste.clear();
    confirmaTeste.sync();
}

void CTestCacic::testChangeVersionRegistry()
{
#ifdef Q_OS_WIN
    //TODO: criar registro falso pro teste.
    QVERIFY(CCacic::changeCacicVersion());
#else
    QSKIP("Teste desnecessário nessa plataforma");
#endif

}

void CTestCacic::testConvertDouble()
{
    double number = 4.0905;

    QString converted = CCacic::convertDouble(number);

    QVERIFY(converted.toDouble() == number);
}

void CTestCacic::testGetTest()
{
    bool ok;
    QJsonObject envio;
    envio["computador"] = OCacicComp.toJsonObject();
    //    qDebug() << envio;
    OCacicComm->comm("/ws/neo/getTest", &ok, envio, true);
    QVERIFY(ok);
}

void CTestCacic::testGetConfig()
{
    bool ok;
    QJsonObject configEnvio;
    configEnvio["computador"] = oColeta.getOComputer().toJsonObject();
    QJsonObject getConfig = OCacicComm->comm("/ws/neo/config", &ok, configEnvio);
    //    qDebug() << getConfig;
    CCacic::setJsonToFile(getConfig["reply"].toObject(), "getConfig.json");

    QVERIFY(ok);
}

void CTestCacic::testColeta()
{
    if (CCacic::getJsonFromFile("getConfig.json")["agentcomputer"].toObject()["actions"].toObject()["col_soft"].toBool() &&
        CCacic::getJsonFromFile("getConfig.json")["agentcomputer"].toObject()["actions"].toObject()["col_hard"].toBool()){
        oColeta.configuraColetas();
        oColeta.run();
        oColeta.waitToCollect();
        QVERIFY(!oColeta.toJsonObject()["software"].toObject().isEmpty() &&
                !oColeta.toJsonObject()["hardware"].toObject().isEmpty());
    } else
        QSKIP("Ação coleta de software desativada.");
}

void CTestCacic::testLogger()
{
    LogCacic logcacic("Cacic-Teste", this->testPath + "/Logs/teste");

    logcacic.escrever(LogCacic::InfoLevel, "Teste do modulo logger.");
    logcacic.escrever(LogCacic::DebugLevel, "Teste do modulo logger.");
    logcacic.escrever(LogCacic::ErrorLevel, "Teste do modulo logger.");
    logcacic.escrever(LogCacic::FatalLevel, "Teste do modulo logger.");
    logcacic.escrever(LogCacic::TraceLevel, "Teste do modulo logger.");
    logcacic.escrever(LogCacic::WarnLevel, "Teste do modulo logger.");


    QFile logFile01(this->testPath + "/Logs/teste/cacic.log");
    if(logFile01.exists()) logFile01.open(QIODevice::ReadOnly);
    QFile logFile02(this->testPath + "/Logs/teste/cacic_debug.log");
    if(logFile02.exists()) logFile02.open(QIODevice::ReadOnly);
    QFile logFile03(this->testPath + "/Logs/teste/cacic_error.log");
    if(logFile03.exists()) logFile03.open(QIODevice::ReadOnly);
    QFile logFile04(this->testPath + "/Logs/teste/cacic_fatalerror.log");
    if(logFile04.exists()) logFile04.open(QIODevice::ReadOnly);
    QFile logFile05(this->testPath + "/Logs/teste/cacic_trace.log");
    if(logFile05.exists()) logFile05.open(QIODevice::ReadOnly);
    QFile logFile06(this->testPath + "/Logs/teste/cacic_warn.log");
    if(logFile06.exists()) logFile06.open(QIODevice::ReadOnly);

    QVERIFY(logFile01.exists() &&
            logFile01.readLine().contains("Teste do modulo logger.") &&
            logFile02.exists() &&
            logFile02.readLine().contains("Teste do modulo logger.") &&
            logFile03.exists() &&
            logFile03.readLine().contains("Teste do modulo logger.") &&
            logFile04.exists() &&
            logFile04.readLine().contains("Teste do modulo logger.") &&
            logFile05.exists() &&
            logFile05.readLine().contains("Teste do modulo logger.") &&
            logFile06.exists() &&
            logFile06.readLine().contains("Teste do modulo logger.")
            );

    logFile01.close();
    logFile02.close();
    logFile03.close();
    logFile04.close();
    logFile05.close();
    logFile06.close();
    QDir(this->testPath + "/Logs/teste/").removeRecursively();
}

void CTestCacic::testDownload()
{
    QJsonObject ftp;

    if(QFile::exists("getConfig.json")){
        ftp = CCacic::getJsonFromFile("getConfig.json")["agentcomputer"].toObject()["metodoDownload"].toObject();
        if (!ftp.isEmpty()) {
            OCacicComm->setFtpPass(ftp["senha"].toString());
            OCacicComm->setFtpUser(ftp["usuario"].toString());
            OCacicComm->fileDownload(ftp["tipo"].toString(),
                                     ftp["url"].toString(),
                                     (ftp["path"].toString().endsWith("/") ? ftp["path"].toString() : ftp["path"].toString() +"/") +
#ifndef Q_OS_WIN
                                     "cacic-service",
#else
                                     "cacic-service.exe",
#endif
                                     this->testPath);
#ifndef Q_OS_WIN
            QFile downloaded(this->testPath + "/cacic-service");
#else
            QFile downloaded(this->testPath + "/cacic-service.exe");
#endif
            QVERIFY( downloaded.open(QIODevice::ReadOnly) &&
                     downloaded.exists()                  &&
                     (downloaded.size() > 0) );
        }
        else {
            QVERIFY2(false, "Não foi possível pegar as informações do arquivo de configuração.");
        }
    } else {
        QVERIFY2(false, "Não existe o arquivo de configuração.");
    }
}

void CTestCacic::testServiceController()
{
#ifdef Q_OS_WIN
    wchar_t serviceName[] = L"cacicdaemon";
    wchar_t *servicePath;
    QString aux = this->testPath + "/cacic-service.exe";
    servicePath = (wchar_t*) malloc(sizeof(wchar_t)*aux.size());
    aux.toWCharArray(servicePath);
    ServiceController service(serviceName);
    if (service.isInstalled()){
        if(service.isRunning()){
            service.stop();
        }
        service.uninstall();
        QThread::sleep(2);
    }

    if (!service.isInstalled()){
        QStringList arg;
        arg << "-i";
        if (QFile::exists(aux)){
            QVERIFY2(service.install(servicePath),
                     "Falha ao instalar serviço.");
            qDebug() << QString::fromStdString(service.getLastError());
        } else {
            QVERIFY2(false, "Binário do serviço não encontrado");
        }
    }
    //garantir a instalação
    QThread::sleep(5);
    if (!service.isRunning()){
        if (!service.start()){
            qDebug() << "Falha ao startar o serviço: " << QString::fromStdString(service.getLastError());
            QVERIFY(false);
        }
        //garantir a inicialização
        QThread::sleep(3);
    }

    if (service.uninstall()){
        QVERIFY(true);
    } else {
        qDebug() << "Falha ao desinstalar o serviço: " << QString::fromStdString(service.getLastError());
        QVERIFY(false);
    }
#else
    QSKIP("Teste desnecessário nessa plataforma");
#endif
}

void CTestCacic::testStopCheckCacicService()
{
#ifdef Q_OS_WIN
    bool ok;
    wchar_t serviceName[] = L"CheckCacic";
    ServiceController *service = new ServiceController(serviceName);
    if (service->isRunning())
        ok =service->stop();
    QThread::sleep(3);
    QVERIFY(ok);
#else
    QSKIP("Teste desnecessário nessa plataforma");
#endif
}

void CTestCacic::testStartProcess()
{
    QString modulo = "/usr/share/cacic/mapacacic";
    QString workDir= "/usr/share/cacic/";
    QMutex *mutex;
    mutex = new QMutex(QMutex::Recursive);
    mutex->lock();
    CacicThread *cacicthread = new CacicThread(workDir);
    cacicthread->setCMutex(mutex);
    cacicthread->setModuloDirPath(modulo);
    cacicthread->setNomeModulo(modulo.split("/").last());
    cacicthread->run();
    qDebug() << cacicthread->getLastError();
    QVERIFY(true);
}

void CTestCacic::testEnviaColeta()
{
    bool ok;
    QJsonObject coletaEnvio = oColeta.toJsonObject();
    CCacic::setJsonToFile(oColeta.toJsonObject(), this->testPath + "/coleta.json");
    //    qDebug() << coletaEnvio;
    OCacicComm->comm("/ws/neo/coleta", &ok, coletaEnvio, true);
    QVERIFY(ok);
}

void CTestCacic::testEnviaLog()
{
    LogCacic *logcacic = new LogCacic("Cacic-Teste", this->testPath + "/Logs/teste");
    bool ok = false;
    QStringList logLvls;
    logLvls.append("ErrorLevel");
    logLvls.append("InfoLevel");

    QJsonObject jsonObject;
    CACIC_Computer oComputer;

    jsonObject["computador"] = oComputer.toJsonObject();

    foreach ( QString stringLvl, logLvls ){

        LogCacic::CacicLogLevel level;
        try {
           level = logcacic->levelName2Value( stringLvl );
        } catch(int) {
            QFAIL("Invalid logLevel.");
        }

        if ( level == LogCacic::InfoLevel ||
             level == LogCacic::ErrorLevel) {

            QFile *logFile;
            logFile = new QFile(logcacic->resolverEnderecoArquivo(level));

            QJsonArray logLevelArray;
            if (logFile->exists() &&
                logFile->open(QIODevice::ReadOnly)) {

                QStringList linesList;
                QTextStream stream(logFile);

                while ( !stream.atEnd() ) {
                    linesList.append( stream.readLine() );
                }

                logFile->close();
//                delete logFile;

                for ( int i = linesList.size()-1; i > linesList.size()-N_LOGS_ENVIO-1; i-- ) {
                    QString timestamp = linesList.at(i).mid(1, linesList.at(i).indexOf("]")-1);
                    QString message = linesList.at(i).mid(linesList.at(i).indexOf("]")+2);

                    QJsonObject lineJson;
                    lineJson["timestamp"] = QJsonValue::fromVariant(timestamp);
                    lineJson["message"] = QJsonValue::fromVariant(message);
                    logLevelArray.append( lineJson );

                }
            }
            if( level == LogCacic::InfoLevel )
                jsonObject["logInfo"] = logLevelArray;
            if (level == LogCacic::ErrorLevel )
                jsonObject["logError"] = logLevelArray;

        }
    }

    if (!jsonObject.isEmpty()){

        CacicComm *OCacicComm = new CacicComm(LOG_DAEMON, this->testPath);
        OCacicComm->setUrlGerente("http://teste.cacic.cc");
//        OCacicComm->setUrlGerente("http://localhost:8080");
        OCacicComm->setUsuario("cacic");
        OCacicComm->setPassword("cacic123");

        QJsonObject retornoColeta;

        logcacic->escrever(LogCacic::InfoLevel, QString("Enviando logs ao gerente."));
        retornoColeta = OCacicComm->comm(ROTA_LOG, &ok, jsonObject , false);

        QVERIFY(ok);
    }

}

void CTestCacic::testGetModulesValues()
{
    bool ok = true;
    oCheckModules = new CheckModules(this->testPath, "teste");
    oCheckModules->start();
    QVariantMap modules = oCheckModules->getModules();
    QVariantMap::const_iterator i = modules.constBegin();
    if (!modules.empty()) {
        do {
            QFile modulo(this->testPath + "/" + i.key());
//            qDebug() << i.key() << " exists: " << modulo.exists();
            ok = modulo.exists() && ok;
            i++;
        } while (i!=modules.constEnd());
    }

    QVERIFY(ok);
}

void CTestCacic::cleanupTestCase()
{
    CCacic::deleteFile("getConfig.json");
    CCacic::deleteFolder(this->testPath);
}
