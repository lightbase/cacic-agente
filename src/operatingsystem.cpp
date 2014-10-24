#include "operatingsystem.h"

OperatingSystem::OperatingSystem()
{
    this->nomeOs = this->coletaNomeOs();
    this->idOs = this->coletaIdOs();
}


/*pegarOS
 * @return: int;
 *      retorna um id referente a versão do SO.
 * 48 = Windows XP
 * 128 = Windows Vista
 * 144 = Windows 7
 * 160 = Windows 8
 * 176 = Windows 8.1
 * [enum] = Linux
 */
int OperatingSystem::coletaIdOs(){
#if defined (Q_OS_WIN)
    return QSysInfo::WindowsVersion;
#elif defined (Q_OS_LINUX)

    ConsoleObject console;
    QStringList catOutput = console("cat /etc/*release").split("\n");

    QString line;
    foreach(line, catOutput) {
        if(line.contains("PRETTY_NAME")) {
            if( line.contains("Arch"))
                return LINUX_ARCH;
            else if( line.contains("Debian"))
                return LINUX_DEBIAN;
            else if( line.contains("Ubuntu"))
                return LINUX_UBUNTU;
        }
    }

#else
    return -1;
#endif
}

QString OperatingSystem::coletaNomeOs()
{
#if defined(Q_OS_WIN)
    QStringList params;
    QJsonValue osName;
    params << "Name";
    osName = wmi::wmiSearch("Win32_OperatingSystem", params);
    if (!osName.isNull()){
        QString retorno = osName.toObject()["Name"].toString();
        retorno = retorno.left(retorno.indexOf("|"));
        return retorno;
    }
#elif defined(Q_OS_LINUX)
    ConsoleObject console;
    QStringList catOutput = console("cat /etc/*release").split("\n");

    QString line;
    foreach(line, catOutput) {
        if(line.contains("PRETTY_NAME")) {
            QStringList split = line.split("=");

            QString nomeDistro = split[1].mid(1, split[1].size()-2 );
            return nomeDistro;
        }
    }
#endif
      return "";

}

QJsonObject OperatingSystem::toJsonObject()
{
    QJsonObject json;
    json["idOs"] = QJsonValue::fromVariant(QVariant::fromValue(this->idOs));
    json["nomeOs"] = QJsonValue::fromVariant(this->nomeOs);

    return json;
}

QString OperatingSystem::getNomeOs()
{
    return this->nomeOs;
}

int OperatingSystem::getIdOs() const
{
    return this->idOs;
}
