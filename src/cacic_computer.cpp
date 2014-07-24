#include "cacic_computer.h"

CACIC_Computer::CACIC_Computer(QObject *parent) :
  QObject(parent)
{
    os  = pegarOS();
    networkInterface = networkInterfacesRunning();
}

QList<QList<std::string>> CACIC_Computer::networkInterfacesRunning(){
    QNetworkInterface interface;
    QList<std::string> lista;
    QList<QList<std::string>> todasInterfaces;

    foreach (QNetworkInterface in, interface.allInterfaces()) {
        if (!(bool)(in.flags() & QNetworkInterface::IsLoopBack) &
            !(bool)(in.flags() & QNetworkInterface::IsPointToPoint) &
             (bool)(in.flags() & QNetworkInterface::IsRunning)){
            lista.append(in.humanReadableName().toStdString());
            lista.append(in.hardwareAddress().toStdString());
            foreach (QNetworkAddressEntry ae, in.addressEntries()){
                if (ae.ip().scopeId() == ""){
                    lista.append(ae.ip().toString().toStdString());
                } else {
                    lista.append(ae.ip().toString().toStdString());
                }
            }
            todasInterfaces.append(lista);
            lista.clear();
        }
    }
    return todasInterfaces;
}

std::string CACIC_Computer::pegarOS(){
  QString text;
  QStringList environment = QProcess::systemEnvironment();
  foreach (text, environment) {
        if (text.contains("OS=", Qt::CaseInsensitive)){
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



