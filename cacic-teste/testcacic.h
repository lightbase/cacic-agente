/*-----------------------------------------------------------------------------------------
 *
 * Project Cacic Agente
 *    File created by Lightbase
 *
 * Developers: Eric Menezes Noronha (eric.m.noronha@lightbase.com.br); GitHub: ericmenezes
 *             Thiago Rocha         (thiago.rocha@lightbase.com.br)  ;
 *
 *-----------------------------------------------------------------------------------------*/
#ifndef TESTEINSTALLCACIC_H
#define TESTEINSTALLCACIC_H

#include <cacic_comm.h>
#include <ccacic.h>
#include <cacic_computer.h>
#include <cacic_software.h>
#include <cacic_hardware.h>
#include <console.h>
#include <operatingsystem.h>
#include <QtTest/QtTest>
#include <QDebug>
#include <QList>
#include <QProcess>
#include <QStringList>
#include <iostream>
#include <QtNetwork/QtNetwork>
#include <QJsonObject>
#include <ccoleta.h>
#include <logcacic.h>
#include <checkmodules.h>
#include <servicecontroller.h>

class CTestCacic : public QObject
{
  Q_OBJECT
public:
  explicit CTestCacic(QObject *parent = 0);

private:
    CacicComm *OCacicComm;
    CACIC_Computer OCacicComp;
    cacic_software OCacicSoftware;
    cacic_hardware OCacicHardware;
    QString testPath;
    QString testIniPath;
    QJsonObject session;
    QString cripTeste;
    CColeta oColeta;
    CheckModules *oCheckModules;
//    cacicD OcacicD;

signals:

public slots:

private slots:
    void initTestCase();
    void testCommStatus();
//    void testGetAndSetValueFromFile();
//    void testGetValueFromTags();
    void testCreateFolder();
    void testDeleteFolder();
    void testDeleteFile();
    void testProcFind();
    void testInterfaceDeRede();
    void testConsole();
    void testPegarUsu();
    void testcoletaIdOs();
    void testpegarOS();
    void testLogin();
    void testSslConnection();
//    void testEnCrypt();
//    void testDeCrypt();
    void testCacicCompToJsonObject();
    void testOperatingSystemToJsonObject();
    void testJsonToFile();
    void testJsonFromFile();
    void testSetRegistry();
    void testGetValueFromRegistry();
    void testRemoveRegistry();
    void testConvertDouble();
    void testGetTest();
    void testGetConfig();
    void testDownload();
    void testGetModulesValues();
    void testColeta();
    void testLogger();
    void testEnviaColeta();
    void testServiceController();
    void testStopCheckCacicService();
    void cleanupTestCase();
};

#endif // TESTEINSTALLCACIC_H
