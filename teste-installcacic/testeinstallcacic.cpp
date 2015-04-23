#include "testeinstallcacic.h"

QTEST_MAIN(testeInstallcacic)

testeInstallcacic::testeInstallcacic(QObject *parent) :
    QObject(parent)
{

    quint16 port = 8080;
    daemon = new HttpDaemon(port);
    daemon->resume();
}

void testeInstallcacic::initTestCase()
{
    icsa = new InstallCacicSA("google.com","cacic","cacic123");
    sc   = new ServiceController(L"FakeServiceCacic");
    this->icsa->setPort(80);

    hash = new std::string("CERTO");
}

void testeInstallcacic::testNaoInstalado()
{
    if (!this->icsa->registryExists(HKEY_LOCAL_MACHINE, L"SOFTWARE\\FakeMsi\\msi")){
        if (this->icsa->downloadService("rota","./cacic-service")){
            QVERIFY(this->icsa->installService());
        } else {
            QVERIFY2(false, "Não conseguiu baixar o servico.");
        }
    } else {
        QVERIFY(true);
    }
}

void testeInstallcacic::testGetHashFromFile()
{
    std::string hash = this->icsa->getHashFromFile("teste-installcacic.exe");

    QVERIFY (hash!="");
}

void testeInstallcacic::testConfig()
{
    std::string hash;
    hash = "24212151";

    // Testa primeiro com o hash errado. Deve retornar a operação de atualização
    this->icsa->setHashLocal(hash);

    // Busca configuração de hash no servidor
    this->icsa->getConfig();

    // Compara os dois. Devem ser diferentes
    QVERIFY(!this->icsa->comparaHash());

    // Agora testa com o hash certo. Deve retornar que está atualiza
    this->icsa->setHashLocal(*this->hash);
    this->icsa->getConfig();

    // Compara os dois. Devem ser iguais
    QVERIFY(this->icsa->comparaHash());

}

void testeInstallcacic::testMsiInstalado()
{
    //registro simulando instalação msi
    QVariantMap registry;
    registry["teste"] = true;
    CCacic::setValueToRegistry("FakeMsi", "msi", registry);
    if (!this->icsa->registryExists(HKEY_LOCAL_MACHINE, L"SOFTWARE\\FakeMsi\\msi")){
        QVERIFY2(false, "Registro de instalação não encontrado");
    } else {
        //Pega informações do gerente
        if(this->icsa->getConfig()){
            //corrige possíveis erros que impeça o serviço de subir
            QVERIFY(this->icsa->verificaServico());
        } else {
            QVERIFY2(false, "Nao foi possivel pegar as informações necessárias");
        }
    }
}

void testeInstallcacic::testServico()
{
    if (!sc->isInstalled() || !sc->isRunning()){
        //verifica se existe o binário install-cacic na pasta bin, se não executa MSI.
        QVERIFY(icsa->installCacic());
    }
    QVERIFY2(false, "Servico instalado ou rodando");
}

void testeInstallcacic::testAtualizacao()
{
    QVERIFY(false);
}

void testeInstallcacic::testCacic26()
{
    QVERIFY(icsa->deleteCacic26());
}

void testeInstallcacic::testCacic28()
{
    QVERIFY(icsa->deleteCacic28());
}

void testeInstallcacic::verificaRegistro()
{
    QVERIFY(icsa->registryExists(HKEY_LOCAL_MACHINE, L"SOFTWARE\\FakeMsi\\msi"));
}

void testeInstallcacic::cleanupTestCase()
{
    CCacic::removeRegistry("FakeMsi", "msi");
    // Para servidor fake
    daemon->resume();

}
