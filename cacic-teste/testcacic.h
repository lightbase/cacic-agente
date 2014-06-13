#ifndef TESTINSTALLCACIC_H
#define TESTINSTALLCACIC_H

#include <QtTest/QtTest>

class CTestCacic : public QObject
{
    Q_OBJECT
public:
    explicit CTestCacic(QObject *parent = 0);

signals:

public slots:

private slots:
    void firstTest();
    void installcacicTest();
};

#endif // TESTINSTALLCACIC_H
