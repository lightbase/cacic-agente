#ifndef CACIC_HARDWARE_H
#define CACIC_HARDWARE_H
#include <ccacic.h>
#include <QtCore>
#include <QJsonArray>
#include <cmath>
#include <ccacic.h>
#include <console.h>
#include <operatingsystem.h>
#include "QLogger.h"
#ifdef Q_OS_WIN
    #include <wmi.h>
#elif defined(Q_OS_LINUX)
    #include <unistd.h>
#endif

class cacic_hardware
{
public:
    cacic_hardware();
    ~cacic_hardware();
    void iniciaColeta();
    QJsonObject toJsonObject();

private:
#ifdef Q_OS_WIN
    QJsonObject coletaWin();

#elif defined(Q_OS_LINUX)
    ConsoleObject console;

    QJsonObject coletaLinux();
    void coletaLinuxOperatingSystem(QJsonObject &hardware);
    void coletaLinuxMem(QJsonObject &hardware, const QJsonObject &component);
    void coletaLinuxCpu(QJsonObject &hardware, const QJsonObject &component);
    void coletaLinuxPci(QJsonObject &hardware, const QJsonObject &pciJson);
    void coletaLinuxIO(QJsonObject &hardware, const QJsonObject &ioJson);
    void coletaGenericPartitionInfo(QJsonObject &newPartition, const QJsonObject &partitionObject);
    void coletaLinuxBios(QJsonObject &hardware);
    void coletaLinuxMotherboard(QJsonObject &hardware);
    void coletaLinuxIsNotebook(QJsonObject &hardware);
    void coletaLinuxPrinters(QJsonObject &hardware);
#endif

    CCacic oCacic;
    QJsonObject coletaHardware;
    QLogger::QLoggerManager *logManager;
};

#endif // CACIC_HARDWARE_H
