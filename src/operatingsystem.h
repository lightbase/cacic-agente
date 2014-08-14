#ifndef OPERATINGSYSTEM_H
#define OPERATINGSYSTEM_H

#include <QtCore>
#include <QSysInfo>

class OperatingSystem
{
private:
    int idSo;

    int pegarOS();
public:
    OperatingSystem();

    enum OsList {
        MAC,
        LINUX_DEBIAN,
        LINUX_UBUNTU,
        LINUX_ARCH
    };
    static const enum OsList OsList;

    int getIdSo() const;
    void setIdSo(int value);
};

#endif // OPERATINGSYSTEM_H
