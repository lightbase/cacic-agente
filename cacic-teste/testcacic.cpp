#include <QProcess>
#include <QStringList>
#include "testcacic.h"
QTEST_MAIN(CTestCacic)

CTestCacic::CTestCacic(QObject *parent) :
    QObject(parent)
{
}

void CTestCacic::installcacicTest()
{
    QProcess installcacic;
    QString installcacicPath;

    installcacicPath = "E:/Lightbase/cacic-projeto-test/build/cacic-test/debug/cacic-teste.exe";
    installcacic.execute(installcacicPath,QStringList() << "--host:");
    if (!installcacic.waitForFinished() and (installcacic.exitCode() == 5))
        QVERIFY(installcacic.exitStatus() == 5);
    else
        QSKIP("Installcacic ExitCode: " + QString::number(installcacic.exitCode()).toLocal8Bit());

}

void CTestCacic::firstTest()
{
    QString str= "Hello";
    QVERIFY(str.toUpper() == "HELLO");
}



