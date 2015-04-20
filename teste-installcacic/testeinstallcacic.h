#ifndef TESTEINSTALLCACIC_H
#define TESTEINSTALLCACIC_H

#include <QtTest>
#include <QDebug>
#include <iostream>
#include <ccacic.h>
#include <../installcacic-standalone/installcacicsa.h>

class testeInstallcacic : public QObject
{
    Q_OBJECT
public:
    explicit testeInstallcacic(QObject *parent = 0);

signals:

public slots:

private slots:
    void initTestCase();
    void verificaRegistro();
    void cacicNaoInstalado();
    void cleanupTestCase();
};

#endif // TESTEINSTALLCACIC_H
