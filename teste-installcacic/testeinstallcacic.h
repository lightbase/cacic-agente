#ifndef TESTEINSTALLCACIC_H
#define TESTEINSTALLCACIC_H

#include <QtTest>
#include <QDebug>
#include <iostream>
#include <ccacic.h>
#include <../installcacic-standalone/installcacicsa.h>
#include <http-daemon.h>

class testeInstallcacic : public QObject
{
    Q_OBJECT
public:   
    explicit testeInstallcacic(QObject *parent = 0);

private:
   HttpDaemon *daemon;

signals:

public slots:

private slots:
    void initTestCase();
    void verificaRegistro();
    void cacicNaoInstalado();
    void cleanupTestCase();
};

#endif // TESTEINSTALLCACIC_H
