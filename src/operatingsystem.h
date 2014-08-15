#ifndef OPERATINGSYSTEM_H
#define OPERATINGSYSTEM_H

#include <QtCore>
#include <QString>
#include <QStringList>
#include <QSysInfo>

#include <ccacic.h>
#include <console.h>

class OperatingSystem
{
private:
    int idOs;
    int coletaIdOs();
    QString nomeOs;
    QString coletaNomeOs();
public:
    OperatingSystem();
    QJsonObject toJsonObject();
    enum OsList {
        MAC,
        LINUX_DEBIAN,
        LINUX_UBUNTU,
        LINUX_ARCH
    };
    static const enum OsList OsList;

    QString getNomeOs();
    int getIdOs() const;
};

#endif // OPERATINGSYSTEM_H
