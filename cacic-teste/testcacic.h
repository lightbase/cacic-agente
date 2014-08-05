#ifndef TESTINSTALLCACIC_H
#define TESTINSTALLCACIC_H

#include <cacic_comm.h>
#include <ccacic.h>
#include <cacic_computer.h>
#include <QtTest/QtTest>
#include <QDebug>
#include <QList>
#include <QProcess>
#include <QStringList>
#include <iostream>
#include <csslcomm.h>

class CTestCacic : public QObject
{
    Q_OBJECT
public:
    explicit CTestCacic(QObject *parent = 0);

private:
    CacicComm OCacicComm;
    CCacic OCacic;
    CACIC_Computer OCacicComp;
    QString testPath;
    QString testIniPath;
    CSslComm OSslComm;

signals:

public slots:

private slots:
    void testCommStatus();
    void testComm();
    void testGetAndSetValueFromFile();
    void testGetValueFromTags();
    void testEnCrypt();
    void testDeCrypt();
    void testCreateFolder();
    void testDeleteFolder();
    void testDeleteFile();
    void testInterfaceDeRede();
    void testpegarOS();
    void testSslConnection();
};

#endif // TESTINSTALLCACIC_H
