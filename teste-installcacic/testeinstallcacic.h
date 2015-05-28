#ifndef TESTEINSTALLCACIC_H
#define TESTEINSTALLCACIC_H

#include <../installcacic-standalone/installcacicsa.h>
#include <QtTest>
#include <QDebug>
#include <iostream>
#include <fstream>
#include <ccacic.h>
#include <servicecontroller.h>
#include <QCoreApplication>

class testeInstallcacic : public QObject
{
    Q_OBJECT
public:   
    explicit testeInstallcacic(QObject *parent = 0);

private:
    InstallCacicSA *icsa;
    ServiceController *sc;
    std::string *hash;
    std::string service_download;
    std::string msi_download;
    std::string path;
    const char *so;
    const char *user;
    double codigo_erro;

signals:

public slots:

private slots:
    void initTestCase();
    void initTestDeleteFolder();
    void testHttpCommunication();
    void testConfig();
    void testNaoInstalado();
    void testGetHashFromFile();
    void testDownloadFile();
    void testMsiInstalado();
    void testServico();
    void testAtualizacao();
    void testCacic26();
    void testCacic28();
    void verificaRegistro();
    void testErro();
    void testInstallDir();
    void testLogErro();
    void cleanupTestCase();
};

#endif // TESTEINSTALLCACIC_H
