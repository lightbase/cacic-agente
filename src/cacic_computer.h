#ifndef CACIC_COMPUTER_H
#define CACIC_COMPUTER_H

#include <QObject>
#include <QStringList>
#include <QtSerialPort/QtSerialPort>
#include <QtSerialPort/QSerialPortInfo>
#include <QtNetwork/QtNetwork>
#include <QHostAddress>

class CACIC_Computer : public QObject
{
  Q_OBJECT
public:
  explicit CACIC_Computer(QObject *parent = 0);
  std::string pegarIPInterfaceDeRede();
  std::string pegarMACInterfaceDeRede();
  std::string pegarOS();
  void setOs(const std::string &value);
  std::string getIp() const;
  std::string getMac() const;
  std::string getOs() const;

private:
  std::string ip;
  std::string mac;
  std::string os;

signals:

public slots:


};

#endif // CACIC_COMPUTER_H
