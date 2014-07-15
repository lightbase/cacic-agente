#include <QProcess>
#include <QStringList>
#include "testcacic.h"
#include <iostream>

QTEST_MAIN(CTestCacic)

CTestCacic::CTestCacic(QObject *parent) :
    QObject(parent)
{
}

void CTestCacic::testGetStatus200()
{
    CacicComm a;

    QVERIFY(a.commStatus("") == 200);
}





