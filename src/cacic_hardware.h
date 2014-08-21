#ifndef CACIC_HARDWARE_H
#define CACIC_HARDWARE_H
#include <ccacic.h>
#include <wbemidl.h>
#include <querysink.h>
#include <iostream>
using namespace std;
#include <comutil.h>
# pragma comment(lib, "wbemuuid.lib")

class cacic_hardware
{
public:
    cacic_hardware();
    void iniciaColeta();
    QJsonObject toJsonObject();

private:
    QJsonObject coletaHardware;
    int wmi();

    QJsonObject coletaWin();
    QJsonObject coletaLinux();
};

#endif // CACIC_HARDWARE_H
