#ifndef CACIC_COMPUTER_H
#define CACIC_COMPUTER_H

#include <QObject>
#include <QStringList>
#include <QtSerialPort/QtSerialPort>
#include <QtSerialPort/QSerialPortInfo>
#include <QtNetwork/QtNetwork>
#include <QSysInfo>

#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>

class CACIC_Computer
{
public:
    CACIC_Computer();

    int getOs() const;
    std::string getUser() const;
    QList<QVariantMap> getNetworkInterface() const;
    QList<QVariantMap> networkInterfacesRunning();
    QJsonObject toJsonObject();

    enum OsList {
        WIN_XP,
        WIN_VISTA,
        WIN_7,
        WIN_8,
        WIN_8_1,
        MAC,
        LINUX_DEBIAN,
        LINUX_UBUNTU,
        LINUX_ARCH
    };
    static const enum OsList OsList;

private:
    int pegarOS();
    std::string pegarUsu();

    QList<QVariantMap> networkInterface;
    int os;
    std::string usuario;

};

Q_DECLARE_METATYPE(CACIC_Computer)
Q_DECLARE_METATYPE(CACIC_Computer*)
#endif // CACIC_COMPUTER_H
