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
 * @return: int;
 *      retorna um id referente a versão do SO.
 * 48 = Windows XP
 * 128 = Windows Vista
 * 144 = Windows 7
 * 160 = Windows 8
 * 176 = Windows 8.1
 * 200 = Linux
 */
int CACIC_Computer::pegarOS(){
#if defined (Q_OS_WIN)
    return QSysInfo::WindowsVersion;
#elif defined (Q_OS_LINUX)
    return 200;
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
      if (text.contains("USER=", Qt::CaseInsensitive)
             || text.contains("USERNAME=", Qt::CaseInsensitive) ){
          QStringList split = text.split("=");
          return split[1].toStdString();
        }
    }
    return false;
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

QList<QList<std::string>> CACIC_Computer::getNetworkInterface() const
{
    return networkInterface;
}
