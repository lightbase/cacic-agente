#ifndef TESTINSTALLCACIC_H
#define TESTINSTALLCACIC_H
#include "../src/CACIC_comm.h"
#include "../src/ccacic.h"
#include <QtTest/QtTest>
#include <QDebug>

class CTestCacic : public QObject
{
    Q_OBJECT
public:
    explicit CTestCacic(QObject *parent = 0);

private:
    CacicComm OCacicComm;
    CCacic OCacic;
    QString testPath;
    QString testIniPath;

signals:

public slots:

private slots:
    void testCommStatus();
    void testComm();
    void testGetAndSetValueFromFile();
    void testGetValueFromTags();
    void testCreateFolder();
    void testDeleteFolder();
    void testDeleteFile();
};

#endif // TESTINSTALLCACIC_H
