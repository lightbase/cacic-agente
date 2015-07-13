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
        if(line.contains("DISTRIB_DESCRIPTION")){
            if( line.contains("Arch"))
                return LINUX_ARCH;
            else if( line.contains("Debian", Qt::CaseInsensitive))
                return LINUX_DEBIAN;
            else if( line.contains("Ubuntu", Qt::CaseInsensitive))
                return LINUX_UBUNTU;
            else if( line.contains("Elementary",Qt::CaseInsensitive))
                return LINUX_ELEMENTARY;
        }
    }
    return -1;
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
            QString version = line.split("=").takeLast();

            if(version.at(0) == QChar('"'))
                return version.split('"').at(1);
            else
                return version;
//            return line.split("=").takeLast().split('"').takeAt(1);
        }else if (line.contains("DISTRIB_RELEASE")){
            return line.split("=").takeLast();
        }
    }
    return QString();
#endif
    return QString();
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
        retorno = retorno.left(retorno.indexOf("|"));
//        qDebug() << "metodo antigo: " << retorno.trimmed();
        return retorno.trimmed();
    } else {
        Computer comp;
        QString retorno = QString(comp.getSo().c_str());
//        qDebug() << "metodo novo: " << retorno.trimmed();
        if (!retorno.isEmpty()){
            return "";
        } else {
            return retorno.trimmed();
        }
    }
#elif defined(Q_OS_LINUX)
    ConsoleObject console;
    QString nomeDistro;

    QStringList split = console("cat /etc/*release | grep DISTRIB_DESCRIPTION").split("=");
//    nomeDistro = split[1].mid(1, split[1].trimmed().size()-2 ).trimmed();

    if (split.size() > 1) {
        nomeDistro = split[1].mid(1,split[1].indexOf("\"", split[1].indexOf("\"") + 1) - 1);
    } else {
        nomeDistro = "Unknown";
    }
//    qDebug() << "----------------" << nomeDistro << "--------------------";
    QStringList consoleOutput = console("uname -i").split("\n");

    if(consoleOutput.contains("unknown"))
        consoleOutput = console("uname -m").split("\n");

    if(consoleOutput.contains("x86_64")){
        nomeDistro.append("-x86_64");
    } else if (consoleOutput.contains("i386") ||
               consoleOutput.contains("i486") ||
               consoleOutput.contains("i586") ||
               consoleOutput.contains("i686")  ){
        nomeDistro.append("-x86");
    }
//    qDebug () << ">>>>>>>>>>>>>>>>>>>>>>>>>" << nomeDistro << "<<<<<<<<<<<<<<<<";
    return nomeDistro;
#endif
    return "";

}

/*********************************************
 * Transforma todos os dados da classe em json
 *********************************************/
QJsonObject OperatingSystem::toJsonObject()
{
    //Verifica os dados novamente antes de gerar outro JSON.
    if (this->nomeOs.isEmpty() || this->nomeOs.isNull()){
        this->nomeOs = this->coletaNomeOs();
    }
    if (this->idOs == -1){
        this->idOs = this->coletaIdOs();
    }
    if (this->nomeOs.isEmpty() || this->nomeOs.isNull()) {
        return QJsonObject();
    }
    QJsonObject json;
    json["idOs"] = QJsonValue::fromVariant(QVariant::fromValue(this->idOs));
    json["nomeOs"] = QJsonValue::fromVariant(this->nomeOs);
#ifdef Q_OS_WIN
    QStringList param;
    param << "SystemUpTime";
    json["tipo"] = QJsonValue::fromVariant(QString("windows"));
    json["upTime"] = QJsonValue::fromVariant(wmi::wmiSearch("Win32_PerfFormattedData_PerfOS_System", param).toObject()["SystemUpTime"].toString().toInt());
#else
    ConsoleObject console;
    json["tipo"] = QJsonValue::fromVariant(QString("linux"));
    json["upTime"] = QJsonValue::fromVariant(static_cast<int> (console("cat /proc/uptime | awk '{print $1}'").toDouble()));
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
