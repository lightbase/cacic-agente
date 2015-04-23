#ifndef TESTEINSTALLCACIC_H
#define TESTEINSTALLCACIC_H

#include <../installcacic-standalone/installcacicsa.h>
#include <QtTest>
#include <QDebug>
#include <iostream>
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

signals:

public slots:

private slots:
    void initTestCase();
    void testHttpCommunication();
    void testNaoInstalado();
    void testGetHashFromFile();
    void testConfig();
    void testMsiInstalado();
    void testServico();
    void testAtualizacao();
    void testCacic26();
    void testCacic28();
    void verificaRegistro();
    void cleanupTestCase();
};

#endif // TESTEINSTALLCACIC_H
