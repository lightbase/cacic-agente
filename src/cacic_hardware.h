#ifndef CACIC_HARDWARE_H
#define CACIC_HARDWARE_H
#include <QtCore>
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
    QJsonObject  toJsonObject();

private:
    QJsonObject coletaWin();
    QJsonObject coletaLinux();

    CCacic oCacic;
    QJsonObject coletaHardware;
};

#endif // CACIC_HARDWARE_H
