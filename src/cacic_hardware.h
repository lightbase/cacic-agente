#ifndef CACIC_HARDWARE_H
#define CACIC_HARDWARE_H
#include <ccacic.h>
#include <querysink.h>
#include <QtCore>
#include <QJsonArray>
#include <QJsonArray>
#include <cmath>
#include <ccacic.h>
#include <console.h>
#include <operatingsystem.h>

#if defined(Q_OS_WIN)
    #define _WIN32_DCOM
    #include <iostream>
    //using namespace std;
    #include <wbemidl.h>
    #include <windows.h>
    # pragma comment(lib, "wbemuuid.lib")

#endif

class cacic_hardware
{
public:
    cacic_hardware();
    void iniciaColeta();
    QJsonObject toJsonObject();
    QJsonObject coletaWin();
    QJsonObject coletaLinux();

private:
    int wmi();
    QJsonObject coletaWin();
    QJsonObject coletaLinux();
    void coletaLinuxMem(QJsonObject &hardware, const QJsonObject &component);
    void coletaLinuxCpu(QJsonObject &hardware, const QJsonObject &component);
    void coletaLinuxPci(QJsonObject &hardware, const QJsonObject &pciJson);

    CCacic oCacic;
    QJsonObject coletaHardware;
};

#endif // CACIC_HARDWARE_H
