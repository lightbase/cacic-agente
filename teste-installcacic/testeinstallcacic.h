#ifndef TESTEINSTALLCACIC_H
#define TESTEINSTALLCACIC_H

#include <QtTest>
#include <QDebug>
#include <iostream>
#include <fstream>
#include <servicecontroller.h>
#include <QCoreApplication>
#include <ccacic.h>
#include <../installcacic-standalone/installcacicsa.h>

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
    void testDeleteFolder();
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
    void testInstallDir();
    void testLogErro();
    void testErro();
    void testGetSo();
    void cleanupTestCase();
};

#endif // TESTEINSTALLCACIC_H
