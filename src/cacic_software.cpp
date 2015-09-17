#include "cacic_software.h"

cacic_software::cacic_software()
{
}

void cacic_software::iniciaColeta()
{
#ifdef Q_OS_WIN
    this->coletaSoftware = coletaWin();
#elif defined(Q_OS_LINUX)
    this->coletaSoftware = coletaLinux();

#endif
}

#if defined(Q_OS_WIN)
/***************************************************************
 * Realiza a coleta de softwares do Windows por meio do regedit.
 ***************************************************************/
using namespace voidrealms::win32;
QJsonObject cacic_software::coletaWin()
{
    QJsonObject softwaresJson;
    QStringList regedit;
    //No windows, ele armazena os dados em 2 locais diferentes se for x64. Um para programas x86 e outro pra x64.
    regedit.append("SOFTWARE\\Wow6432Node\\Microsoft\\Windows\\CurrentVersion\\Uninstall\\");
    regedit.append("SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Uninstall\\");
    foreach(QString registry, regedit){
        VRegistry reg;
        reg.OpenKey(HKEY_LOCAL_MACHINE, registry);
        QStringList keys = reg.enum_Keys();
        qDebug() << _exceptions;
        foreach(QString key, keys){
            if(!_exceptions.contains(key) ||
               (_exceptions.contains(key) && !_exceptions[key].isEmpty())){
                QVariantMap software;
                VRegistry subReg;
                subReg.OpenKey(HKEY_LOCAL_MACHINE, registry + key);
                if (!subReg.get_REG_SZ("DisplayName").isEmpty())
                    software["description"] = subReg.get_REG_SZ("DisplayName");
                if (!subReg.get_REG_SZ("Publisher").isEmpty())
                    software["publisher"] = subReg.get_REG_SZ("Publisher");
                if (!subReg.get_REG_SZ("InstallLocation").isEmpty())
                    software["installLocation"] = subReg.get_REG_SZ("InstallLocation");
                if (!subReg.get_REG_SZ("InstallDate").isEmpty())
                    software["installDate"] = subReg.get_REG_SZ("InstallDate");
                if (!subReg.get_REG_SZ("URLInfoAbout").isEmpty())
                    software["url"] = subReg.get_REG_SZ("URLInfoAbout");
                if (!subReg.get_REG_EXPAND_SZ("UninstallString").isEmpty())
                    software["uninstallString"] = subReg.get_REG_EXPAND_SZ("UninstallString");
                if (!subReg.get_REG_EXPAND_SZ("QuietUninstallString").isEmpty())
                    software["quietUninstallString"] = subReg.get_REG_EXPAND_SZ("QuietUninstallString");
                if (!subReg.get_REG_SZ("DisplayVersion").isEmpty())
                    software["version"] = subReg.get_REG_SZ("DisplayVersion");
                software["name"] = key;

                if(_exceptions.contains(key)){
                    foreach(QString value, _exceptions[key]){
                        qDebug() << value;
                        software.remove(value);
                    }
                }
                softwaresJson[key] = QJsonObject::fromVariantMap(software);
            }
        }
    }
    return softwaresJson;
}

#elif defined(Q_OS_LINUX)
QJsonObject cacic_software::coletaLinux()
{
    OperatingSystem operatingSystem;

    if( operatingSystem.getIdOs() == OperatingSystem::LINUX_ARCH ) {
        return coletaArch();
    } else /*if ( operatingSystem.getIdOs() == OperatingSystem::LINUX_DEBIAN ||
                operatingSystem.getIdOs() == OperatingSystem::LINUX_UBUNTU )*/ {
        return coletaDebian();
    }

    return QJsonObject();
}

QJsonObject cacic_software::coletaArch()
{
    ConsoleObject console;
    QJsonObject softwaresJson;

    QStringList packages = console("pacman -Qe").split("\n");

    foreach(QString package, packages) {
        QString packageName = package.split(" ")[0];
        if (!(packageName.isEmpty() || packageName.isNull()) &&
                (!_exceptions.contains(packageName)
                || !(_exceptions.contains(packageName)
                && _exceptions[packageName].isEmpty()))){
            QJsonObject packageJson;

            QStringList packageInfo = console(QString("pacman -Qi ").append(packageName)).split("\n");

            packageJson["name"] = QJsonValue::fromVariant(QString(packageName));
            foreach(QString line, packageInfo) {
                if(line.contains("Version") &&
                        !(_exceptions.contains(packageName)
                        && _exceptions[packageName].contains("version")))
                    packageJson["version"] = line.split(":")[1].mid(1);
                if(line.contains("Description") &&
                        !(_exceptions.contains(packageName)
                        && _exceptions[packageName].contains("description")))
                    packageJson["description"] = line.split(":")[1].mid(1);
                if(line.contains("URL")) {
                    QStringList url = line.split(":");
                    QString urlString;

                    for(int i = 1 ; i < url.size() ; ++i){
                        urlString.append(url[i]);
                        if(i == 1 ) urlString.append(":");
                    }
                    if(!(_exceptions.contains(packageName)
                         && _exceptions[packageName].contains("url")))
                        packageJson["url"] = urlString.mid(1);
                    if (!(_exceptions.contains(packageName)
                          && _exceptions[packageName].contains("publisher")))
                        packageJson["publisher"] = urlString.mid(1);
                }
    //            installSize não existe na coleta do Windows.
    //            if(line.contains("Installed size"))
    //                packageJson["installSize"] = line.split(":")[1].mid(1);
                if(line.contains("Install Date") &&
                        !(_exceptions.contains(packageName)
                        && _exceptions[packageName].contains("installDate")))
                    packageJson["installDate"] = line.split(":")[1].mid(1);
            }

            if (!(_exceptions.contains(packageName)
                  && _exceptions[packageName].contains("installLocation"))) {

                QString location = console(QString("whereis ").append(packageName)).split("\n").at(0);
                if(!location.split(":").at(1).isEmpty()) {
                    location = location.split(":").at(1).mid(1);
                    packageJson["installLocation"] = location;
                }
            }
            softwaresJson[packageName] = packageJson;
        }
    }
    return softwaresJson;
}

QJsonObject cacic_software::coletaDebian()
{
    ConsoleObject console;
    QJsonObject softwaresJson;

    QStringList packages = console("dpkg --get-selections | grep -v '\^lib\\|\^fonts'").split("\n");

    foreach(QString package, packages) {
        QString packageName = package.split("\t")[0];
        if (!(packageName.isEmpty() || packageName.isNull()) &&
                (!_exceptions.contains(packageName)
                || !(_exceptions.contains(packageName)
                && _exceptions[packageName].isEmpty()))){
            QJsonObject packageJson;

            QStringList packageInfo = console(QString("apt-cache show ").append(packageName)).split("\n");

            packageJson["name"] = QJsonValue::fromVariant(QString(packageName));
            foreach(QString line, packageInfo) {
                if(line.contains("Version:") &&
                        !(_exceptions.contains(packageName)
                        && _exceptions[packageName].contains("version")))
                    packageJson["version"] = line.split(":")[1].mid(1);
                if(line.contains("Description-en:") &&
                        !(_exceptions.contains(packageName)
                        && _exceptions[packageName].contains("description"))){
                    packageJson["description"] = line.split(":")[1].mid(1);
                }
                if(line.contains("Homepage:")) {
                    QStringList url = line.split(":");
                    QString urlString;

                    for(int i = 1 ; i < url.size() ; ++i){
                        urlString.append(url[i]);
                        if(i == 1 ) urlString.append(":");
                    }

                    if (!(_exceptions.contains(packageName)
                          && _exceptions[packageName].contains("url")))
                        packageJson["url"] = urlString.mid(1);
                    if (!(_exceptions.contains(packageName)
                          && _exceptions[packageName].contains("publisher")))
                        packageJson["publisher"] = urlString.mid(1);
                }
    //            installSize não existe na coleta do Windows.
    //            if(line.contains("Installed-Size:"))
    //                packageJson["installSize"] = line.split(":")[1].mid(1);

            }

            if (!packageName.isEmpty() &&
                    !(_exceptions.contains(packageName)
                    && _exceptions[packageName].contains("installLocation"))){
                QString treatedPackageName = packageName;
                if(treatedPackageName.contains("amd64") || treatedPackageName.contains("i386"))
                    treatedPackageName = treatedPackageName.split(":").at(0);

                QString location = console(QString("whereis ").append(treatedPackageName)).split("\n").at(0);
                if(!location.split(":").at(1).isEmpty()) {
                    location = location.split(":").at(1).mid(1);
                    packageJson["installLocation"] = location;
                }
            }
            softwaresJson[packageName] = packageJson;
        }

//        int counterPackages = softwaresJson.size();
    }

    return softwaresJson;
}
#endif
QHash<QString, QStringList> cacic_software::exceptions() const
{
    return _exceptions;
}

void cacic_software::setExceptionClasses(const QHash<QString, QStringList> &exceptions)
{
    _exceptions = exceptions;
}


QJsonObject cacic_software::toJsonObject()
{
    return coletaSoftware;
}
