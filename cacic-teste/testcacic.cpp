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

void CTestCacic::testCreateFolder()
{
    ConsoleObject console;
    qDebug() << QString::number(static_cast<int> (console("cat /proc/uptime | awk '{print $1}'").toDouble()));
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

void CTestCacic::testDeleteFolder()
{
    QDir folder(testPath);
    CCacic::deleteFolder(testPath);
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
    QVERIFY(CCacic::setJsonToFile(json, "teste.json"));
}

void CTestCacic::testJsonFromFile()
{
    //    qDebug() << CCacic::getJsonFromFile("teste123.json");
    QVERIFY(CCacic::getJsonFromFile("teste.json")["teste"].toString() == "teste");
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

void CTestCacic::testColetaSoftware()
{
    if (CCacic::getJsonFromFile("getConfig.json")["agentcomputer"].toObject()["actions"].toObject()["col_soft"].toBool()){
        oColeta.configuraColetas();
        oColeta.run();
        oColeta.waitToCollect();
        QVERIFY(!oColeta.toJsonObject()["software"].toObject().isEmpty());
    } else
        QSKIP("Ação coleta de software desativada.");
}

void CTestCacic::testColetaHardware()
{
    if (CCacic::getJsonFromFile("getConfig.json")["agentcomputer"].toObject()["actions"].toObject()["col_hard"].toBool())
        QVERIFY(!oColeta.toJsonObject()["hardware"].toObject().isEmpty());
    else
        QSKIP("Ação coleta de hardware desativada.");
}

void CTestCacic::testLogger()
{
    LogCacic logcacic("Cacic-Teste", Identificadores::ENDERECO_PATCH_CACIC+"/Logs/teste");

    logcacic.escrever(LogCacic::InfoLevel, "Teste do modulo logger.");
    logcacic.escrever(LogCacic::DebugLevel, "Teste do modulo logger.");
    logcacic.escrever(LogCacic::ErrorLevel, "Teste do modulo logger.");
    logcacic.escrever(LogCacic::FatalLevel, "Teste do modulo logger.");
    logcacic.escrever(LogCacic::TraceLevel, "Teste do modulo logger.");
    logcacic.escrever(LogCacic::WarnLevel, "Teste do modulo logger.");


    QFile logFile01(Identificadores::ENDERECO_PATCH_CACIC+"/Logs/teste/cacic.log");
    if(logFile01.exists()) logFile01.open(QIODevice::ReadOnly);
    QFile logFile02(Identificadores::ENDERECO_PATCH_CACIC+"/Logs/teste/cacic_debug.log");
    if(logFile02.exists()) logFile02.open(QIODevice::ReadOnly);
    QFile logFile03(Identificadores::ENDERECO_PATCH_CACIC+"/Logs/teste/cacic_error.log");
    if(logFile03.exists()) logFile03.open(QIODevice::ReadOnly);
    QFile logFile04(Identificadores::ENDERECO_PATCH_CACIC+"/Logs/teste/cacic_fatalerror.log");
    if(logFile04.exists()) logFile04.open(QIODevice::ReadOnly);
    QFile logFile05(Identificadores::ENDERECO_PATCH_CACIC+"/Logs/teste/cacic_trace.log");
    if(logFile05.exists()) logFile05.open(QIODevice::ReadOnly);
    QFile logFile06(Identificadores::ENDERECO_PATCH_CACIC+"/Logs/teste/cacic_warn.log");
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
    QDir(Identificadores::ENDERECO_PATCH_CACIC+"/Logs/teste/").removeRecursively();
}

void CTestCacic::testDownload()
{
    QJsonObject ftp;

    ftp = CCacic::getJsonFromFile("getConfig.json")["agentcomputer"].toObject()["metodoDownload"].toObject();

    OCacicComm->setFtpPass(ftp["senha"].toString());
    OCacicComm->setFtpUser(ftp["usuario"].toString());
    OCacicComm->fileDownload(ftp["tipo"].toString(),
                             ftp["url"].toString(),
                             (ftp["path"].toString().endsWith("/") ? ftp["path"].toString() : ftp["path"].toString() +"/") +
                             "cacic-service",
                             "");
    QFile downloaded("cacic-service");
    QVERIFY( downloaded.open(QIODevice::ReadOnly) &&
             downloaded.exists()                  &&
             (downloaded.size() > 0) );
}

void CTestCacic::testServiceController()
{
#ifdef Q_OS_WIN
    wchar_t serviceName[] = L"cacicdaemon";
    wchar_t *servicePath;
    QString aux = QDir::currentPath() + "/cacic-service.exe";
    servicePath = (wchar_t*) malloc(sizeof(wchar_t)*aux.size());
    aux.toWCharArray(servicePath);
    ServiceController service(serviceName);
    if (!service.isInstalled()){
        QStringList arg;
        arg << "-i";
        if (QProcess::execute(QString::fromWCharArray(servicePath), arg) != 0){
            qDebug() << "Falha ao instalar serviço.";
            QVERIFY(false);
        }
    }
    if (!service.isRunning()){
        if (!service.start()){
            qDebug() << "Falha ao startar o serviço: " << QString::fromStdString(service.getLastError());
            QVERIFY(false);
        }
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
    bool ok;
    wchar_t serviceName[] = L"CheckCacic";
    ServiceController *service = new ServiceController(serviceName);
    if (service->isRunning())
        ok =service->stop();
    QThread::sleep(3);
    qDebug() << "PASSOU";
    QVERIFY(ok);

    qDebug() << "PASSOU";
}

void CTestCacic::testEnviaColeta()
{
    bool ok;
    QJsonObject coletaEnvio = oColeta.toJsonObject();
    CCacic::setJsonToFile(oColeta.toJsonObject(), "coleta.json");
    //    qDebug() << coletaEnvio;
    OCacicComm->comm("/ws/neo/coleta", &ok, coletaEnvio, true);
    QVERIFY(ok);
}

void CTestCacic::testGetModulesValues()
{
    bool ok = true;
    oCheckModules = new CheckModules(QDir::currentPath(), "teste");
    oCheckModules->start();
    qDebug() << "passed";
    QVariantMap modules = oCheckModules->getModules();
    QVariantMap::const_iterator i = modules.constBegin();
    if (!modules.empty()) {
        do {
            QFile modulo("./temp/" + i.key());
            ok = modulo.exists() && ok;
            i++;
        } while (i!=modules.constEnd());
    }

    QDir dir("./temp");
    dir.setFilter(QDir::Files | QDir::Hidden | QDir::NoSymLinks | QDir::Executable);
    dir.setSorting(QDir::Size | QDir::Reversed);

    QFileInfoList list = dir.entryInfoList();
    for (int i = 0; i<list.size(); i++){
        QFile novoModulo(list.at(i).filePath());
        if (QFile::exists(QDir::currentPath() + "/" + list.at(i).fileName())){
            QFile::remove(QDir::currentPath() + "/" + list.at(i).fileName());
        }
        novoModulo.copy(QDir::currentPath() + "/" + list.at(i).fileName());
        novoModulo.close();
    }

    QVERIFY(ok);
}

void CTestCacic::cleanupTestCase()
{
    CCacic::deleteFile("gpl-2.0.txt");
    CCacic::deleteFile("configRequest.json");
    CCacic::deleteFile("teste.json");
    //    CCacic::deleteFile("getConfig.json");
    CCacic::deleteFolder("./temp");
    CCacic::deleteFile("./install-cacic");
    CCacic::deleteFile("./gercols");
    //    CCacic::deleteFile("./coleta.json");
}
