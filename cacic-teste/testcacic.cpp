#include "testcacic.h"

QTEST_MAIN(CTestCacic)

CTestCacic::CTestCacic(QObject *parent) :
    QObject(parent)
{
}

void CTestCacic::initTestCase()
{
    this->OCacicComm = CacicComm("http://teste.cacic.cc",
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
    OCacicComm.setUrlSsl("https://10.1.0.137/cacic/web/app_dev.php");
    OCacicComm.setUsuario("cacic");
    OCacicComm.setPassword("cacic123");
    this->testPath = QDir::currentPath() + "/teste";
    this->testIniPath = testPath + "/teste.ini";
    QVariantMap jsonMap;
    jsonMap["session"] = "lakdhfalkfhsaklfhasfhsl";
    this->session = QJsonObject::fromVariantMap(jsonMap);
}

void CTestCacic::testCreateFolder()
{
    QVERIFY(OCacic.createFolder(testPath));
}

void CTestCacic::testGetAndSetValueFromFile()
{
    OCacic.setValueToFile("Teste", "teste", "Valor de teste", testIniPath);
    QVERIFY( OCacic.getValueFromFile("Teste", "teste", testIniPath) == "Valor de teste");
}

void CTestCacic::testGetValueFromTags()
{
    QString value = "blablab[teste]Valor de teste[/teste]feihgj";
    QVERIFY(OCacic.getValueFromTags(value, "teste") == "Valor de teste");
}

void CTestCacic::testCommStatus()
{
    QVERIFY(OCacicComm.commStatus());
}

void CTestCacic::testComm()
{
    bool ok;
    if (OCacicComm.commStatus()){
        QJsonObject jsonreply = OCacicComm.comm("/ws/get/test", &ok);
//        qDebug() << jsonreply["codestatus"].toString();
        QVERIFY(OCacic.getValueFromTags(jsonreply["reply"].toString(), "Comm_Status", "<>") == QString("OK"));
    } else
        QSKIP("Teste de comunicação negativo!");
}

void CTestCacic::testDeleteFile()
{
    QDir file(testIniPath);
    OCacic.deleteFile(testIniPath);
    QVERIFY(!file.exists());
}

void CTestCacic::testDeleteFolder()
{
    QDir folder(testPath);
    OCacic.deleteFolder(testPath);
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
    ConsoleObject console;
#if defined(Q_OS_LINUX)
    QVERIFY(console("echo teste").toStdString() == "teste\n");
#elif defined(Q_OS_WIN)
    QVERIFY(console("echo teste").toStdString() == "teste");
#else
    QVERIFY(false);
#endif
}

void CTestCacic::testPegarUsu(){
    QVERIFY(OCacicComp.getUser() != "");
}

void CTestCacic::testJsonValueFromJsonString()
{
    QVERIFY(OCacic.jsonValueFromJsonString("{\"nome\":\"teste\"}", "nome").toString() == "teste");
}

void CTestCacic::testLogin(){
    bool ok;
    QJsonObject jsonReply = OCacicComm.login(&ok);
    QJsonObject sessionvalue = jsonReply["reply"].toObject();
    OCacic.setChaveCrypt(sessionvalue["chavecrip"].toString());
    QVERIFY(ok);
}

void CTestCacic::testSslConnection()
{
    bool ok;
    QJsonObject json = OCacicComm.comm("", &ok, QJsonObject(), true);
    QJsonValue jsonvalue = (!json["codestatus"].isNull()) ?
                            json["codestatus"] :
                            QJsonValue::fromVariant(-1);
//    qDebug() << jsonvalue.toDouble();
    QVERIFY(jsonvalue.toDouble() == 200 || jsonvalue.toDouble() == 302);
}

void CTestCacic::testEnCrypt(){
    std::string IV = "0123456789123456"; //iv nunca se repete para a mesma senha.
    std::string input = "aqui vai a url que sera encriptada";
    OCacic.setChaveCrypt("testecript123456");
    this->cripTeste = OCacic.enCrypt(input, IV);
    QVERIFY(!this->cripTeste.isEmpty() && !this->cripTeste.isNull());
}

void CTestCacic::testDeCrypt(){
    std::string IV = "0123456789123456asas"; //iv nunca se repete para a mesma senha.
    std::string input = this->cripTeste.toStdString();
    QVERIFY(OCacic.deCrypt(input, IV) == "aqui vai a url que sera encriptada");

}

void CTestCacic::testInstallCacicStart()
{
//    char *argva[] = {"program name", "-host=teste.cacic.cpp", "-user=asda", "-password=qwesd", NULL};
//    int argca = sizeof(argva) / sizeof(char*) - 1;
//    QCoreApplication a(argca, argva);

//    InstallCacic *oInstallCacic = new InstallCacic(&a);

//    QStringList args;
//    for (int i = 0; i<argca; i++)
//        args.append(argva[i]);
//    // This will cause the application to exit when
//    // the task signals finished.
//    QObject::connect(oInstallCacic, SIGNAL(finished()), &a, SLOT(quit()));

//    // This will run the task from the application event loop.
//    QMetaObject::invokeMethod(oInstallCacic, "run", Qt::QueuedConnection, Q_ARG(QStringList, args), Q_ARG(int, argca));
    QVERIFY(false);
}

void CTestCacic::testCacicCompToJsonObject()
{
//    qDebug() << OCacicComp.toJsonObject();
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
    QVERIFY(OCacic.setJsonToFile(json, "teste.json"));
}

void CTestCacic::testJsonFromFile()
{
    QVERIFY(OCacic.getJsonFromFile("teste.json")["teste"].toString() == "teste");
}

void CTestCacic::testStartService()
{
    bool ok;
    QString exitStatus;
#ifdef Q_OS_WIN
    exitStatus = OCacic.startProcess("../../install-cacic/debug/install-cacic.exe", true, &ok);
#else
    exitStatus = OCacic.startProcess("../../install-cacic/debug/install-cacic", &ok);
#endif
    QVERIFY(ok);
}

void CTestCacic::testReadConfig()
{

//    // Inicializa um arquivo de configuração stub
//    // que seria parecido com o recebido do Gerente
//    QJsonObject configJson;
//    QJsonObject configHardware;

//    configHardware["computador"] = OCacicComp.toJsonObject();

//    configJson["hardware"] = configHardware;

//    OCacic.setJsonToFile(configJson,"configRequest.json");

    // Leitura do arquivo de configuração

    QVERIFY(false);
}

void CTestCacic::testSetRegistry()
{
    QVariantMap valueMap;
    valueMap["teste1"] = QString("Teste 1");
    valueMap["teste2"] = QString("Teste2");
    OCacic.setValueToRegistry("Lightbase", "Teste", valueMap);
    QSettings confirmaTeste("Lightbase", "Teste");
    QVERIFY(confirmaTeste.value("teste1") == QVariant("Teste 1"));
}

void CTestCacic::testRemoveRegistry()
{
    OCacic.removeRegistry("Lightbase", "Teste");
    QSettings confirmaTeste("Lightbase", "Teste");
    QVERIFY(confirmaTeste.allKeys().isEmpty());
    confirmaTeste.clear();
    confirmaTeste.sync();
}

void CTestCacic::cleanupTestCase()
{
    OCacic.deleteFile("configRequest.json");
    OCacic.deleteFile("teste.json");
}
