#ifndef CACIC_COMPUTER_H
#define CACIC_COMPUTER_H

#include <QObject>
#include <QStringList>
#include <QtSerialPort/QtSerialPort>
#include <QtSerialPort/QSerialPortInfo>
#include <QtNetwork/QtNetwork>
#include <QSysInfo>

class CACIC_Computer
{
public:
    CACIC_Computer();

    int pegarOS();
    std::string pegarUsu();
    int getOs() const;
    std::string getUsuario() const;
    QList<QVariantMap> getNetworkInterface() const;
    QList<QVariantMap> networkInterfacesRunning();
    QJsonObject toJsonObject();

private:
    QList<QVariantMap> networkInterface;
    int os;
    std::string usuario;

};

Q_DECLARE_METATYPE(CACIC_Computer)
Q_DECLARE_METATYPE(CACIC_Computer*)
#endif // CACIC_COMPUTER_H
