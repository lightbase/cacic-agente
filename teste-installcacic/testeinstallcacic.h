#ifndef TESTEINSTALLCACIC_H
#define TESTEINSTALLCACIC_H

#include <QtTest>
#include <QDebug>
#include <iostream>
#include <ccacic.h>
#include <../installcacic-standalone/installcacicsa.h>
#include <servicecontroller.h>

class testeInstallcacic : public QObject
{
    Q_OBJECT
public:
    explicit testeInstallcacic(QObject *parent = 0);

private:
    InstallCacicSA *icsa;
    ServiceController *sc;

signals:

public slots:

private slots:
    void initTestCase();
    void testNaoInstalado();
    void testMsiInstalado();
    void testServico();
    void testAtualizacao();
    void testCacic26();
    void testCacic28();
    void verificaRegistro();
    void cleanupTestCase();
};

#endif // TESTEINSTALLCACIC_H
