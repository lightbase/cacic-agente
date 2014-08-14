#ifndef CACIC_COMPUTER_H
#define CACIC_COMPUTER_H

#include <QObject>
#include <QStringList>
#include <QtSerialPort/QtSerialPort>
#include <QtSerialPort/QSerialPortInfo>
#include <QtNetwork/QtNetwork>
#include <QSysInfo>
#include <operatingsystem.h>

class CACIC_Computer
{
public:
    CACIC_Computer();

    OperatingSystem getOs() const;
    std::string getUser() const;
    QList<QVariantMap> getNetworkInterface() const;
    QList<QVariantMap> networkInterfacesRunning();
    QJsonObject toJsonObject();

private:

    OperatingSystem oOperatingSystem;
    int pegarOS();
    std::string pegarUsu();

    QList<QVariantMap> networkInterface;
    std::string usuario;

};

Q_DECLARE_METATYPE(CACIC_Computer)
Q_DECLARE_METATYPE(CACIC_Computer*)
#endif // CACIC_COMPUTER_H
