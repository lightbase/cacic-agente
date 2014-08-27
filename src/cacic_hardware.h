#ifndef CACIC_HARDWARE_H
#define CACIC_HARDWARE_H
#include <ccacic.h>
#include <QtCore>
#include <QJsonArray>
#include <cmath>
#include <ccacic.h>
#include <console.h>
#include <operatingsystem.h>
#ifdef Q_OS_WIN
//    #include <QAxObject>
    #include <windows.h>
#endif

class cacic_hardware
{
public:
    cacic_hardware();
    void iniciaColeta();
    QJsonObject toJsonObject();

private:
#ifdef Q_OS_WIN
    QJsonObject coletaWin();
#elif Q_OS_LINUX
    QJsonObject coletaLinux();
    void coletaLinuxMem(QJsonObject &hardware, const QJsonObject &component);
    void coletaLinuxCpu(QJsonObject &hardware, const QJsonObject &component);
    void coletaLinuxPci(QJsonObject &hardware, const QJsonObject &pciJson);
#endif

    CCacic oCacic;
    QJsonObject coletaHardware;
};

#endif // CACIC_HARDWARE_H
