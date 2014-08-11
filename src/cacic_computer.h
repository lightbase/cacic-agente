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
    std::string getUser() const;
    QList<QList<std::string>> getNetworkInterface() const;
    QList<QList<std::string>> networkInterfacesRunning();

private:
    QList<QList<std::string>> networkInterface;
    int os;
    std::string usu;

};

Q_DECLARE_METATYPE(CACIC_Computer)
Q_DECLARE_METATYPE(CACIC_Computer*)
#endif // CACIC_COMPUTER_H
