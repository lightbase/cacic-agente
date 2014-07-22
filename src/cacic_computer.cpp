#include "cacic_computer.h"

CACIC_Computer::CACIC_Computer(QObject *parent) :
  QObject(parent)
{
}

void CACIC_Computer::setIp(const std::string &value)
{
  ip = value;
}

void CACIC_Computer::setMac(const std::string &value)
{
  mac = value;
}

void CACIC_Computer::setOs(const std::string &value)
{
  os = value;
}

std::string CACIC_Computer::pegarIPInterfaceDeRede() {
  QNetworkInterface interface;
  QList<QHostAddress> result;
  QList<QHostAddress> IpList = interface.allAddresses();
  for (int i = 0; i < IpList.size(); i++){
      if((!IpList.at(i).isLoopback()) & (IpList.at(i).scopeId() == Q_NULLPTR)){
          result.append(IpList.at(i));
          setIp(IpList.at(i).toString().toStdString());
          break;
        }
    }
  return result.at(0).toString().toStdString();
}


std::string CACIC_Computer::pegarMACInterfaceDeRede(){
  foreach (const QNetworkInterface &ni, QNetworkInterface::allInterfaces()) {
      if (!(ni.flags() & ni.IsLoopBack)){
          //qDebug() << ni.hardwareAddress();
          return ni.hardwareAddress().toStdString();
          break;
        }
    }
  return "ERROR MAC";
}

std::string CACIC_Computer::pegarOS(){
  QString text;
  qint32 x=0;
  //QStringList environment = QProcessEnvironment::systemEnvironment().toStringList();
  QStringList environment = QProcess::systemEnvironment();
//  foreach (text, environment) {
//      qDebug() << x << sear;
//      x++;
//    }
}
