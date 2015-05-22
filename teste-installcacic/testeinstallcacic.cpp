#include "testeinstallcacic.h"

QTEST_MAIN(testeInstallcacic)

testeInstallcacic::testeInstallcacic(QObject *parent) :
    QObject(parent)
{

}

void testeInstallcacic::initTestCase()
{
    // Verifica se o serviço de teste está no ar

    icsa = new InstallCacicSA("localhost","cacic","cacic123");
    sc   = new ServiceController(L"FakeServiceCacic");
    this->icsa->setPort(8080);

    hash = new std::string("CERTO");
    service_download = std::string("http://teste.cacic.cc/downloads/cacic/current/windows/cacic-service.exe");
    msi_download = std::string("http://teste.cacic.cc/downloads/cacic/current/windows/Cacic.msi");
    QString tmp_dir = QDir::tempPath().replace("/","\\");
    path = tmp_dir.toStdString();
}

void testeInstallcacic::testHttpCommunication()
{
    QVERIFY2(this->icsa->ping(), "Falha na comunicação com o serviço");
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
    QVERIFY2(!(this->icsa->comparaHash()), "Hash deveria ser diferente mas é igual.");

    // Agora testa com o hash certo. Deve retornar que está atualizado
    this->icsa->setHashLocal(*this->hash);
    this->icsa->getConfig();

    // Compara os dois. Devem ser iguais
//    std::cout << "!Hash local: " << this->icsa->getHashLocal() << "!\n";
//    std::cout << "!Hash remoto: " << this->icsa->getHashRemoto() << "!\n";
    QVERIFY2(this->icsa->comparaHash(), ("Hash deveria ser igual mas é diferente"));

}

void testeInstallcacic::testNaoInstalado()
{
    if (!this->icsa->registryExists(HKEY_LOCAL_MACHINE, L"SOFTWARE\\FakeMsi\\msi")){
        if (this->icsa->downloadMsi(this->msi_download, this->path)){
            QVERIFY(this->icsa->installCacic(this->path+"\\Cacic.msi"));
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
//    qDebug() << QString::fromStdString(hash);
    QVERIFY (hash!="");
}



void testeInstallcacic::testDownloadFile()
{
    std::cout << "Baixando para diretório de arquivos temporários: " << this->path << std::endl;

    QVERIFY(this->icsa->downloadService(this->service_download, this->path));
    this->icsa->setUrl("localhost");
}

void testeInstallcacic::testMsiInstalado()
{
    //registro simulando instalação msi
    QVariantMap registry;
    registry["teste"] = true;
    CCacic::setValueToRegistry("FakeMsi", "msi", registry);

    QVERIFY2(this->icsa->registryExists(HKEY_LOCAL_MACHINE, L"SOFTWARE\\FakeMsi\\msi"),
             "Não foi possível encontrar o registro.");
}

void testeInstallcacic::testServico()
{
    if (!sc->isInstalled() || !sc->isRunning()){
        //verifica se existe o binário install-cacic na pasta bin, se não executa MSI.
        QVERIFY(icsa->verificaServico());
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
    CCacic::deleteFile("./websearch.pdf");
}
