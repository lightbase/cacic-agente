#ifndef TESTINSTALLCACIC_H
#define TESTINSTALLCACIC_H

#include <QtTest/QtTest>

class CTestInstallCacic : public QObject
{
    Q_OBJECT
public:
    explicit CTestInstallCacic(QObject *parent = 0);

signals:

public slots:

private slots:
    void firstTest();
};

#endif // TESTINSTALLCACIC_H
