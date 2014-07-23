#include "cacic_computer.h"

CACIC_Computer::CACIC_Computer(QObject *parent) :
  QObject(parent)
{
    ip  = pegarIPInterfaceDeRede();
    mac = pegarMACInterfaceDeRede();
}

std::string CACIC_Computer::pegarIPInterfaceDeRede() {
  QNetworkInterface interface;
  QList<QHostAddress> IpList = interface.allAddresses();
  for (int i = 0; i < IpList.size(); i++){
      if((!IpList.at(i).isLoopback()) & (IpList.at(i).scopeId() == "")){
          return IpList.at(i).toString().toStdString();
        }
    }
  return "";
}


std::string CACIC_Computer::pegarMACInterfaceDeRede(){
  foreach (const QNetworkInterface &ni, QNetworkInterface::allInterfaces()) {
      if (!(ni.flags() & ni.IsLoopBack)){
          //qDebug() << ni.hardwareAddress();
          return ni.hardwareAddress().toStdString();
      }
    }
  return "";
}

/*
 * getters/setters
 */

std::string CACIC_Computer::getIp() const
{
    return ip;
}
std::string CACIC_Computer::getMac() const
{
    return mac;
}
std::string CACIC_Computer::getOs() const
{
    return os;
}


