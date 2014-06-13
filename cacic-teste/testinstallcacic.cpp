#include "testinstallcacic.h"
QTEST_MAIN(CTestInstallCacic)

CTestInstallCacic::CTestInstallCacic(QObject *parent) :
    QObject(parent)
{
}

void CTestInstallCacic::firstTest()
{
    QString str= "Hello";
    QVERIFY(str.toUpper() == "HELLO");
}



