#include "testeinstallcacic.h"

QTEST_MAIN(testeInstallcacic)

testeInstallcacic::testeInstallcacic(QObject *parent) :
    QObject(parent)
{
}

void testeInstallcacic::initTestCase()
{

}

void testeInstallcacic::cacicNaoInstalado()
{

    QVERIFY(false);
}

void testeInstallcacic::cleanupTestCase()
{

}
