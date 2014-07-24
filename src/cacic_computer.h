#ifndef CACIC_COMPUTER_H
#define CACIC_COMPUTER_H

#include <QObject>
#include <QStringList>
#include <QtSerialPort/QtSerialPort>
#include <QtSerialPort/QSerialPortInfo>
#include <QtNetwork/QtNetwork>

class CACIC_Computer : public QObject
{
  Q_OBJECT
public:
    explicit CACIC_Computer(QObject *parent = 0);

    std::string pegarOS();
    std::string getOs() const;
    QList<QList<std::string>> getNetworkInterface() const;
    QList<QList<std::string>> networkInterfacesRunning();

private:
     QList<QList<std::string>> networkInterface;
    std::string os;

signals:

public slots:


};

#endif // CACIC_COMPUTER_H
