#include "testeinstallcacic.h"

QTEST_MAIN(testeInstallcacic)

testeInstallcacic::testeInstallcacic(QObject *parent) :
    QObject(parent)
{
}

void testeInstallcacic::initTestCase()
{
    QVariantMap registry;
    registry["teste"] = true;
    CCacic::setValueToRegistry("TesteInstallCacic", "teste", registry);
}

void testeInstallcacic::verificaRegistro()
{
    InstallCacicSA icsa;
    QVERIFY(icsa.registryExists(HKEY_LOCAL_MACHINE, L"SOFTWARE\\TesteInstallCacic\\teste"));
}

void testeInstallcacic::cacicNaoInstalado()
{

    QVERIFY(false);
}

void testeInstallcacic::cleanupTestCase()
{
    CCacic::removeRegistry("TesteInstallCacic", "teste");
}
