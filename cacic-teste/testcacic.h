#ifndef TESTINSTALLCACIC_H
#define TESTINSTALLCACIC_H

#include <../src/cacic_comm.h>
#include <../src/ccacic.h>
#include <../src/cacic_computer.h>
#include <QtTest/QtTest>
#include <QDebug>
#include <QList>
#include <QProcess>
#include <QStringList>
#include <iostream>
#include <QtNetwork/QtNetwork>
#include <QJsonObject>

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
  void testPegarUsu();
  void testJsonValueFromJsonString();
  void testpegarOS();
  void testLogin();
  void testSslConnection();
  void testEnCrypt();
  void testDeCrypt();
  void cleanupTestCase();
};

#endif // TESTINSTALLCACIC_H
