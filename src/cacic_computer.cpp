#include "cacic_computer.h"

CACIC_Computer::CACIC_Computer()
{
    os  = pegarOS();
    networkInterface = networkInterfacesRunning();
}

/*NetworkInterfacesRunning
 * @return: QList<QList<std::string>>.
 *      Retorna todas as interfaces com seus IP's, nome e MAC.
 *      Cada uma é salva em uma lista que também será salva em
 *      outra lista que conterá todas as interfaces.
 *
 */
QList<QList<std::string>> CACIC_Computer::networkInterfacesRunning(){
    QNetworkInterface interface;
    QList<std::string> lista;
    QList<QList<std::string>> todasInterfaces;

    foreach (QNetworkInterface in, interface.allInterfaces()) {
        if (!(bool)(in.flags() & QNetworkInterface::IsLoopBack) &&
            !(bool)(in.flags() & QNetworkInterface::IsPointToPoint) &&
             (bool)(in.flags() & QNetworkInterface::IsRunning)){
//            qDebug() << in.humanReadableName() << "\n";
            lista.append(in.humanReadableName().toStdString());
//            qDebug() << in.hardwareAddress() << "\n";
            lista.append(in.hardwareAddress().toStdString());
            foreach (QNetworkAddressEntry ae, in.addressEntries()){
                if (ae.ip().scopeId() == ""){
                    lista.append(ae.ip().toString().toStdString());
                } else {
                    lista.append(ae.ip().toString().toStdString());
                }
//                qDebug() << ae.ip().toString() << "\n";
            }
            todasInterfaces.append(lista);
            lista.clear();
        }
    }
    return todasInterfaces;
}

/*pegarOS
 * @return: std::string;
 *      retorna uma string o qual dirá qual so é.
 *
 */
std::string CACIC_Computer::pegarOS(){
  QString text;
  QStringList environment = QProcess::systemEnvironment();
  foreach (text, environment) {
        if (text.contains("OS=", Qt::CaseInsensitive)){
            if ((QSysInfo::windowsVersion() >= QSysInfo::WV_WINDOWS8) &&
                (QSysInfo::windowsVersion() < QSysInfo::WV_WINDOWS8_1));
//                qDebug () << "win 8";
            else if (QSysInfo::windowsVersion() >= QSysInfo::WV_WINDOWS8_1)
//                qDebug() << "win 8.1";
            return text.mid(text.indexOf("=")+1).toStdString();
        }
        //implementar o if pra quando for linux.
    }
  return "";
}

/*
 * getters/setters
 */

std::string CACIC_Computer::getOs() const
{
    return os;
}
QList<QList<std::string>> CACIC_Computer::getNetworkInterface() const
{
    return networkInterface;
}

