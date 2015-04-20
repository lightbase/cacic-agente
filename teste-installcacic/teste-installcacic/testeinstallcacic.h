#ifndef TESTEINSTALLCACIC_H
#define TESTEINSTALLCACIC_H

#include <QtTest>
#include <QDebug>
#include <iostream>

class testeInstallcacic : public QObject
{
    Q_OBJECT
public:
    explicit testeInstallcacic(QObject *parent = 0);

signals:

public slots:

private slots:
    void initTestCase();
    void cacicNaoInstalado();
    void cleanupTestCase();
};

#endif // TESTEINSTALLCACIC_H
