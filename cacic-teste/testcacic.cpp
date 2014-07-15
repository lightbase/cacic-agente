#include <QProcess>
#include <QStringList>
#include "testcacic.h"
#include <iostream>

QTEST_MAIN(CTestCacic)

CTestCacic::CTestCacic(QObject *parent) :
    QObject(parent)
{
}

void CTestCacic::testCommStatus()
{
    QVERIFY(a.commStatus(QString ("http://teste.cacic.cc")));
}

void CTestCacic::testComm()
{
    if (a.commStatus("http://teste.cacic.cc"))
        QVERIFY(a.comm("http://teste.cacic.cc/ws/get/test") != QString(""));
    else
        QSKIP("Teste de comunicação negativo!");
}





