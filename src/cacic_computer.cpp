#include "cacic_computer.h"

CACIC_Computer::CACIC_Computer()
{
    os  = pegarOS();
    networkInterface = networkInterfacesRunning();
    usu = pegarUsu();
}

/*NetworkInterfacesRunning
 * @return: QList<QList<std::string>>.
 *      Retorna todas as interfaces com seus IP's, nome e MAC.
 *      Cada uma é salva em uma lista que também será salva em
 *      outra lista que conterá todas as interfaces.
 *
 */
QList<QVariantMap> CACIC_Computer::networkInterfacesRunning(){
    QNetworkInterface interface;
    QVariantMap mapInterface;
    QList<QVariantMap> listaMap;

    foreach (QNetworkInterface in, interface.allInterfaces()) {
        if (!(bool)(in.flags() & QNetworkInterface::IsLoopBack) &&
                !(bool)(in.flags() & QNetworkInterface::IsPointToPoint) &&
                (bool)(in.flags() & QNetworkInterface::IsRunning)){
            //            qDebug() << in.humanReadableName() << "\n";
            mapInterface["nome"] = in.humanReadableName();
            //            qDebug() << in.hardwareAddress() << "\n";
            mapInterface["mac"]  = in.hardwareAddress();
            foreach (QNetworkAddressEntry ae, in.addressEntries()){
                if (ae.ip().scopeId() == ""){
                    mapInterface["ipv4"] = ae.ip().toString();
                } else {
                    mapInterface["ipv6"] = ae.ip().toString();
                }
                //                qDebug() << ae.ip().toString() << "\n";
            }
            listaMap.append(mapInterface);
            mapInterface.clear();
        }
    }
//    foreach (QVariantMap each, listaMap){

//        foreach (QVariant eachValue, each.values())
//            qDebug() << eachValue.toString();
//    }

    return listaMap;
}

/*pegarOS
 * @return: int;
 *      retorna um id referente a versão do SO.
 * 48 = Windows XP
 * 128 = Windows Vista
 * 144 = Windows 7
 * 160 = Windows 8
 * 176 = Windows 8.1
 * 200 = Linux
 * 0   = unkown
 */
int CACIC_Computer::pegarOS(){
#if defined (Q_OS_WIN)
    return QSysInfo::WindowsVersion;
#elif defined (Q_OS_LINUX)
    return 200;
#else
    return 0;
#endif
}

/*
 * PegarUsu()
 * @return: std::string;
 *      retorna o usuario logado no sistema.
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
int CACIC_Computer::getOs() const
{
    return os;
}

std::string CACIC_Computer::getUser() const {
    return usu;
}

QList<QVariantMap> CACIC_Computer::getNetworkInterface() const
{
    return networkInterface;
}
