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
    #include <windows.h>
    #include <ActiveQt/ActiveQt>
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

#elif defined(Q_OS_LINUX)
/* Thiago aqui:
 * Ao menos nos elif's tem que deixar com o "defined()".
 * Por isso antes eu preferia deixar todas essas verificações com ele, mesmo tendo o ifdef.
 */
    QJsonObject coletaLinux();
    void coletaLinuxMem(QJsonObject &hardware, const QJsonObject &component);
    void coletaLinuxCpu(QJsonObject &hardware, const QJsonObject &component);
    void coletaLinuxPci(QJsonObject &hardware, const QJsonObject &pciJson);
#endif

    CCacic oCacic;
    QJsonObject coletaHardware;
};

#endif // CACIC_HARDWARE_H
