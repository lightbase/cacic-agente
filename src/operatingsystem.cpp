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
    //TODO: Colocar no final da string se é 32btis ou 64bits.
    QStringList params;
    QJsonValue osName;
    params << "Name";
    osName = wmi::wmiSearch("Win32_OperatingSystem", params);
    if (!osName.isNull()){
        QString retorno = osName.toObject()["Name"].toString();
        retorno = retorno.left(retorno.indexOf("|")).trimmed();
        return retorno.trimmed();
    }
#elif defined(Q_OS_LINUX)
    ConsoleObject console;
    QStringList consoleOutput;
    QString line;
    QString nomeDistro;

    consoleOutput = console("cat /etc/*release").split("\n");
    foreach(line, consoleOutput) {
        if(line.contains("PRETTY_NAME")) {
            QStringList split = line.split("=");
            nomeDistro = split[1].mid(1, split[1].size()-2 ).trimmed();
        }
    }

    consoleOutput = console("uname -i").split("\n");

    if(consoleOutput.contains("unknown"))
        consoleOutput = console("uname -m").split("\n");

    nomeDistro.append("-"+consoleOutput.at(0));

    return nomeDistro;
#endif
      return "";

}

/*********************************************
 * Transforma todos os dados da classe em json
 *********************************************/
QJsonObject OperatingSystem::toJsonObject()
{
    QJsonObject json;
    json["idOs"] = QJsonValue::fromVariant(QVariant::fromValue(this->idOs));
    json["nomeOs"] = QJsonValue::fromVariant(this->nomeOs);
#ifdef Q_OS_WIN
    json["tipo"] = QJsonValue::fromVariant(QString("windows"));
#else
    ConsoleObject console;
    QStringList consoleOutput;
    consoleOutput = console("uname -i").split("\n");

    if(consoleOutput.contains("unknown"))
        consoleOutput = console("uname -m").split("\n");
    json["tipo"] = QJsonValue::fromVariant(QString("linux") + QString("-") + consoleOutput.at(0));
#endif

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
