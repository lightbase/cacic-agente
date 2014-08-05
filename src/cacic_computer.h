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

    std::string pegarOS();
    std::string getOs() const;
    QList<QList<std::string>> getNetworkInterface() const;
    QList<QList<std::string>> networkInterfacesRunning();

private:
    QList<QList<std::string>> networkInterface;
    std::string os;

};

Q_DECLARE_METATYPE(CACIC_Computer)
Q_DECLARE_METATYPE(CACIC_Computer*)
#endif // CACIC_COMPUTER_H
