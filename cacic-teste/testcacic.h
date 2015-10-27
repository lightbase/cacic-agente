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
#include <QMenu>
#include <QProcess>
#include <QStringList>
#include <QSystemTrayIcon>
#include <iostream>
#include <QtNetwork/QtNetwork>
#include <QJsonObject>
#include <ccoleta.h>
#include <logcacic.h>
#include <checkmodules.h>
#include <servicecontroller.h>
#include <cacicthread.h>
#include <hardwareclasses.h>

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
//    QSystemTrayIcon *sysTray;
//    cacicD OcacicD;

signals:

public slots:

private slots:
    void initTestCase();
    void testClassDestructors();
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
    void testChangeVersionRegistry();
    void testSalvarVersao();
    void testConvertDouble();
//    void testSystemTray();
    void testGetTest();
    void testGetConfig();
    void testDownload();
    void testCModule();
    void testCModuleArray();
    void testGetModulesValues();
//    void testHardwareClasses();
    void testColeta();
    void testLogger();
    void testEnviaColeta();
    void testEnviaLog();
    void testServiceController();
    void testStopCheckCacicService();
    void testStartProcess();
    void cleanupTestCase();

//    void sysTrayClicked(QSystemTrayIcon::ActivationReason reason);
//    void actionClicked();
};

#endif // TESTEINSTALLCACIC_H
