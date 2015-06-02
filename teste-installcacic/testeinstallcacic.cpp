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
    QString tmp_dir = QDir::tempPath().replace("/","\\");
    path = tmp_dir.toStdString();
    user = "João da Silva";
    so = "Windows 7";
    codigo_erro = 90;

    // Modo debug
    icsa->debug = true;
}

void testeInstallcacic::testDeleteFolder()
{
    QString testPath = QString::fromStdString(this->path);
    testPath.append("\\testDel");
    int numExceptFiles = 2;
    std::string testExceptionFiles[numExceptFiles];
    QStringList lDir;
    lDir.append(testPath + "\\teste\\teste1");
    lDir.append(testPath + "\\teste2\\teste1");
    lDir.append(testPath + "\\teste3");
    lDir.append(testPath + "\\teste");
    foreach(QString dir, lDir){
        dir.replace("/","\\");
        QDir qdir(dir);
        qdir.mkpath(dir);
        for(int i = 0; i<4;i++){
            QFile teste(dir + "\\teste" + QString::number(i+1));
            teste.open(QIODevice::ReadWrite);
            teste.write("teste");
            teste.close();
        }
    }
    testExceptionFiles[0] = "teste1";
    testExceptionFiles[1] = "teste3";

    testPath.replace("/", "\\");
    QVERIFY(this->icsa->delFolder(testPath.toStdString(), testExceptionFiles, numExceptFiles));

    QDir dir(testPath);
    dir.removeRecursively();
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
    std::string msi_path = this->path+"\\" + CACIC_MSI;
    if (!this->icsa->registryExists(HKEY_LOCAL_MACHINE, L"SOFTWARE\\FakeMsi\\msi")){
        QVERIFY2(this->icsa->downloadMsi(this->path), "Não consegui baixar o serviço");
        QVERIFY2(this->icsa->fileExists(msi_path), "Arquivo Inexistente");
        QVERIFY(this->icsa->installCacic(msi_path));
    } else {
        QVERIFY(true);
    }

    // Remove MSI
    QVERIFY(this->icsa->removeCacic(msi_path));

    //Exclui arquivo depois da desinstalação pra não dar problema.
    QFile::remove(QString::fromStdString(msi_path));
}

void testeInstallcacic::testGetHashFromFile()
{
    std::string hash = this->icsa->getHashFromFile("teste-installcacic.exe");
//    qDebug() << QString::fromStdString(hash);
    QVERIFY (hash!="");
}



void testeInstallcacic::testDownloadFile()
{
//    std::cout << "Baixando para diretório de arquivos temporários: " << this->path << std::endl;

    QVERIFY2(this->icsa->downloadService(this->path), "Não foi possível baixar o serviço");

    // Apaga arquivo
    std::string full_path = path + std::string("\\") + std::string(CACIC_SERVICE_BIN);
    QFile::remove(QString::fromStdString(full_path));
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

/**
 * @brief testeInstallcacic::testInstallDir
 *
 * Testa criação do diretório de instalação
 *
 */
void testeInstallcacic::testInstallDir()
{
    std::string result = this->icsa->createInstallDir();
    bool found;
    if (result == this->icsa->getInstallDir()) {
        found = true;
    } else {
        found = false;
    }
    QVERIFY2(found, "Erro ao criar o diretório de instalação");

    QVERIFY2(this->icsa->removeInstallDir(), "Erro ao remover o diretório de instalação");
}

/**
 * @brief testeInstallcacic::testLogErro
 *
 * Testa criação do Log de Erros local
 */
void testeInstallcacic::testLogErro()
{
    std::string result = this->icsa->createLogFile();
    const char *message = "Erro de teste!!!";

    QVERIFY2(this->icsa->fileExists(result), "Não foi possível criar o arquivo");

    QVERIFY2(this->icsa->log(this->codigo_erro, this->user, this->so, message, "ERROR"), "Erro no envio do JSON de erros");

    QVERIFY2(this->icsa->log(this->codigo_erro, this->user, this->so, message, "INFO"), "Erro no envio do JSON de info");

    // VErifica conteúdo
    std::ifstream outfile (result.c_str());
    QVERIFY2(outfile.is_open(), "Não foi possível abrir o arquivo");

    // Escreve o conteúdo só pra testar
    std::string line;
    while (std::getline(outfile, line)) {
        std::cout << line << std::endl;
    }
    outfile.close();

    QVERIFY2(this->icsa->removeInstallDir(), "Erro ao remover o diretório de instalação");
}

/**
 * @brief testeInstallcacic::testErro
 *
 * Testa criação do arquivo de erro
 */
void testeInstallcacic::testErro()
{
    const char *message = "Erro de teste!!!";
    QVERIFY2(this->icsa->log(message), "Erro no envio de JSON sem usuário e SO");

    // Agora testa erro de envio registrando todas as informações
    QVERIFY2(this->icsa->log(this->codigo_erro, this->user, this->so, message), "Erro no envio do JSON completo");

    QVERIFY2(this->icsa->removeInstallDir(), "Erro ao remover o diretório de instalação");
}

/**
 * @brief testeInstallcacic::testGetSo
 *
 * Testa identificação do SO na máquina
 */
void testeInstallcacic::testGetSo()
{
    std::string so = this->icsa->getSo();
    std::string vazio = std::string("");
    std::string nso = std::string("Windows Não Identificado");

    std::cout << "SO Encontrado: " << so << std::endl;

    QVERIFY2((so != vazio), "SO Vazio");
    QVERIFY2((so != nso), "Erro na Identificação do SO");
}

void testeInstallcacic::testGetUsuarioSo()
{
    std::string user = this->icsa->getUsuarioSo();
    std::string vazio = std::string("");
    std::string nso = std::string("Usuário não identificado");

    std::cout << "Usuário Encontrado: " << user << std::endl;

    QVERIFY2((user != vazio), "Usuário Vazio");
    QVERIFY2((user != nso), "Erro na Identificação do Usuário");
}

/**
 * @brief testeInstallcacic::testExec
 *
 * Testa função de instalação e remoção completa
 */
void testeInstallcacic::testExec()
{
    QVERIFY2(this->icsa->exec(), "Instalação falhou!!!");

    QVERIFY2(this->icsa->execRemove(), "Remoção falhou falhou!!!");
}

void testeInstallcacic::testGetNetworkInfo()
{
    int n;
    struct networkInfo net[MAX_NETINFO];
    n = this->icsa->getNetworkInfo(net);
    printf("---------------NET INFO-------------\n");
    for (int i = 0; i<n ; i++){
        printf("\tIP Address[%d]:     \t%s\n", i, net[i].ip);
        printf("\tSubnet Mask[%d]:    \t%s\n", i,  net[i].subnetMask);
    }
    printf("TOTAL: %d\n", n);
    printf("------------------------------------\n");
    QVERIFY(n != -1);

    // Agora verifica interface de rede válida
    int result = this->icsa->getValidNetwork(net);
    QVERIFY2((result > 0), "Nenhuma interface de rede válida");

    QVERIFY2((result == 1), "Encontrou o índice certo da interface");

    QVERIFY2((std::string(net[result].ip) != "127.0.0.01"), "Encontrou o localhost. Erro!");

    std::cout << "Interface encontrada: " << net[result].ip << std::endl;
}

void testeInstallcacic::cleanupTestCase()
{
    // Imprime o conteúdo do arquivo
    QFile f(QString::fromStdString(this->icsa->createLogFile()));
    QTextStream in(&f);
    while(!in.atEnd()) {
        QString line = in.readLine();
        std::cout << line.toStdString() << std::endl;
    }

    // Destrói objeto
    delete this->icsa;

    CCacic::removeRegistry("FakeMsi", "msi");
}
