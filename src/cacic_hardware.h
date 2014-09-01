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
#elif defined(Q_OS_LINUX)
    #include <unistd.h>
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
    QJsonValue wmiSearch(QString classe, QStringList params);

#elif defined(Q_OS_LINUX)
    ConsoleObject console;

    QJsonObject coletaLinux();
    void coletaLinuxMem(QJsonObject &hardware, const QJsonObject &component);
    void coletaLinuxCpu(QJsonObject &hardware, const QJsonObject &component);
    void coletaLinuxPci(QJsonObject &hardware, const QJsonObject &pciJson);
    void coletaLinuxBios(QJsonObject &hardware);
    void coletaLinuxMotherboard(QJsonObject &hardware);
#endif

    CCacic oCacic;
    QJsonObject coletaHardware;
};

#endif // CACIC_HARDWARE_H
