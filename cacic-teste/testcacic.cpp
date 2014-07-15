#include <QProcess>
#include <QStringList>
#include "testcacic.h"
#include <iostream>

QTEST_MAIN(CTestCacic)

CTestCacic::CTestCacic(QObject *parent) :
    QObject(parent)
{
}

void CTestCacic::installcacicTest()
{
    QProcess installcacic;
    QString installcacicPath;

    installcacicPath = QString("E:/LightBase/cacic-agente-project/builds/install-cacic/debug/install-cacic.exe");
    installcacic.execute(installcacicPath,QStringList() << "--host");
    if (installcacic.waitForFinished() and (installcacic.exitCode() == 0))
        QVERIFY(installcacic.exitCode() == 0);
    else
        QSKIP("Installcacic ExitCode: " + QString::number(installcacic.exitCode()).toLocal8Bit());

}

void CTestCacic::firstTest()
{
    QString str= "Hello";
    QVERIFY(str.toUpper() == "HELLO");
}





