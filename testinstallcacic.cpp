#include "testinstallcacic.h"
QTEST_MAIN(CTestInstallCacic)

CTestInstallCacic::CTestInstallCacic(QObject *parent) :
    QObject(parent)
{
}

void CTestInstallCacic::testParametros()
{
    QString str= "Hello";
    QVERIFY(str.toUpper() == "HELLO");
}


