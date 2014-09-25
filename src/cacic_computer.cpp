#include "cacic_computer.h"

CACIC_Computer::CACIC_Computer()
{
    networkInterface = networkInterfacesRunning();
    usuario = pegarUsu();
}

/*NetworkInterfacesRunning
 * @return: QList<QList<std::string>>.
 *      Retorna todas as interfaces com seus IP's, nome e MAC.
 *      Cada uma é salva em uma lista que também será salva em
 *      outra lista que conterá todas as interfaces.
 *
 */
QList<QVariantMap> CACIC_Computer::networkInterfacesRunning(){
    QNetworkInterface netInterface;
    QVariantMap mapInterface;
    QList<QVariantMap> listaMap;

    foreach (QNetworkInterface in, netInterface.allInterfaces()) {
        if (!(bool)(in.flags() & QNetworkInterface::IsLoopBack) &&
            !(bool)(in.flags() & QNetworkInterface::IsPointToPoint) &&
            (bool)(in.flags() & QNetworkInterface::IsRunning)){
            mapInterface["nome"] = in.humanReadableName();
            mapInterface["mac"]  = in.hardwareAddress();
            foreach (QNetworkAddressEntry ae, in.addressEntries()){
                if (ae.ip().scopeId() == ""){
                    mapInterface["ipv4"] = ae.ip().toString();
                    mapInterface["netmask_ipv4"] = ae.netmask().toString();
                } else {
                    mapInterface["ipv6"] = ae.ip().toString();
                    mapInterface["netmask_ipv6"] = ae.netmask().toString();
                }
            }
            listaMap.append(mapInterface);
            mapInterface.clear();
        }
    }
    return listaMap;
}

QJsonObject CACIC_Computer::toJsonObject()
{
    QJsonObject json;
    QJsonArray network;
    int count = 1;
    json["usuario"] = QJsonValue::fromVariant(QString::fromStdString(this->usuario));
    json["operatingSystem"] = this->oOperatingSystem.toJsonObject();
    foreach(QVariantMap auxMap, this->getNetworkInterface()){
        network.append(QJsonObject::fromVariantMap(auxMap));
        count++;
    }
    json["networkDevices"] = network;
    return json;
}


/*
 * PegarUsu()
 * @return: std::string;
 *      retorna o usuario logado no sistema. (erro, retorna o usuário que está executando.)
*/
std::string CACIC_Computer::pegarUsu(){
  QString text;
  QStringList environment = QProcessEnvironment::systemEnvironment().toStringList();
  foreach (text, environment) {
      if (text.contains("USER="    , Qt::CaseInsensitive) ||
          text.contains("USERNAME=", Qt::CaseInsensitive) ){
          QStringList split = text.split("=");
//          qDebug() << split[1];
          return split[1].toStdString();
        }
    }
    return "0";
}
/*
 * getters/setters
*/
OperatingSystem CACIC_Computer::getOs() const
{
    return oOperatingSystem;
}

std::string CACIC_Computer::getUser() const {
    return usuario;
}

QList<QVariantMap> CACIC_Computer::getNetworkInterface() const
{
    return networkInterface;
}
