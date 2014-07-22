#ifndef TESTINSTALLCACIC_H
#define TESTINSTALLCACIC_H
#include "../src/CACIC_comm.h"
#include "../src/ccacic.h"
#include "../src/cacic_computer.h"
#include <QtTest/QtTest>
#include <QDebug>
#include <QList>
#include <QProcess>
#include <QStringList>
#include <iostream>
#include <QHostAddress>

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
    void testpegarIPInterfaceDeRede();
    void testpegarMACInterfaceDeRede();
    void testpegarOS();
};

#endif // TESTINSTALLCACIC_H
