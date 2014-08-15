#ifndef TESTINSTALLCACIC_H
#define TESTINSTALLCACIC_H

#include <cacic_comm.h>
#include <ccacic.h>
#include <cacic_computer.h>
#include <console.h>
#include <QtTest/QtTest>
#include <QDebug>
#include <QList>
#include <QProcess>
#include <QStringList>
#include <iostream>
#include <QtNetwork/QtNetwork>
#include <QJsonObject>
#include "../install-cacic/installcacic.h"

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
    QJsonObject session;
    QString cripTeste;

signals:

public slots:

private slots:
  void initTestCase();
  void testCommStatus();
  void testComm();
  void testGetAndSetValueFromFile();
  void testGetValueFromTags();
  void testCreateFolder();
  void testDeleteFolder();
  void testDeleteFile();
  void testInterfaceDeRede();
  void testConsole();
  void testPegarUsu();
  void testJsonValueFromJsonString();
  void testpegarOS();
  void testLogin();
  void testSslConnection();
  void testEnCrypt();
  void testDeCrypt();
  void testInstallCacicStart();
  void testCacicCompToJsonObject();
  void testOperatingSystemToJsonObject();
  void testJsonToFile();
  void testJsonFromFile();
  void testStartService();
  void cleanupTestCase();
};

#endif // TESTINSTALLCACIC_H
