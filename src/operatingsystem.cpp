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

QString OperatingSystem::coletaVersaoOsEmString(){
#if defined (Q_OS_WIN)
    switch (QSysInfo::WindowsVersion) {
    case QSysInfo::WV_NT:
        return QString("Windows NT");
        break;
    case QSysInfo::WV_2000:
        return QString("Windows 2000");
        break;
    case QSysInfo::WV_XP:
        return QString("Windows 2000");
        break;
    case QSysInfo::WV_2003:
        return QString("Windows Server 2003");
        break;
    case QSysInfo::WV_VISTA:
        return QString("Windows Vista");
        break;
    case QSysInfo::WV_WINDOWS7:
        return QString("Windows 7");
        break;
    case QSysInfo::WV_WINDOWS8:
        return QString("Windows 8");
        break;
    case QSysInfo::WV_WINDOWS8_1:
        return QString("Windows 8.1");
        break;
    default:
        break;
    }
#elif defined (Q_OS_LINUX)
    ConsoleObject console;
    QStringList catOutput = console("cat /etc/*release").split("\n");
    QString line;
    foreach(line, catOutput) {
        if(line.contains("VERSION=") && line.contains(",")) {
            return QString(line.split("=").takeLast().split('"').takeAt(1).split(",").takeFirst());
        }else if (line.contains("VERSION=")){
            return line.split("=").takeLast().split('"').takeAt(1);
        }
    }

#else
    return null;
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
    if(consoleOutput.contains("unknown")){
        consoleOutput = console("uname -m").split("\n");
    }
    if(consoleOutput.contains("x86_64")){
        json["tipo"] = QJsonValue::fromVariant(QString("linux") + QString("-") + consoleOutput.at(0));
    } else if(consoleOutput.contains("i386") || consoleOutput.contains("i686")){
        json["tipo"] = QJsonValue::fromVariant(QString("linux") + QString("-x86"));
    }

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
