#ifndef TESTINSTALLCACIC_H
#define TESTINSTALLCACIC_H
#include "../src/CACIC_comm.h"
#include <QtTest/QtTest>

class CTestCacic : public QObject
{
    Q_OBJECT
public:
    explicit CTestCacic(QObject *parent = 0);

    CacicComm a;

signals:

public slots:

private slots:
    void testCommStatus();
    void testComm();
};

#endif // TESTINSTALLCACIC_H
