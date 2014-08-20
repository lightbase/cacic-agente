#include "cacic_hardware.h"

cacic_hardware::cacic_hardware()
{
}

void cacic_hardware::iniciaColeta()
{
#ifdef Q_OS_WIN
    this->coletaHardware = coletaWin();
#elif defined(Q_OS_LINUX)
    this->coletaHardware = coletaLinux();

#endif
}

QJsonObject cacic_hardware::coletaWin()
{
#if defined(Q_OS_WIN)
    // Implementação seguindo as definições na documentação WMI.
#endif
    return QJsonObject();
}

QJsonObject cacic_hardware::coletaLinux()
{
    return QJsonObject();
}
