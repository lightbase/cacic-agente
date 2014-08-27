#include "cacic_software.h"
#ifdef Q_OS_WIN
    using namespace voidrealms::win32;
#endif
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
QJsonObject cacic_software::coletaWin()
{
    QJsonObject softwaresJson;
    QStringList regedit;
    regedit.append("SOFTWARE\\Wow6432Node\\Microsoft\\Windows\\CurrentVersion\\Uninstall\\");
    regedit.append("SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Uninstall\\");
    foreach(QString registry, regedit){
        VRegistry reg;
        reg.OpenKey(HKEY_LOCAL_MACHINE, registry);
        QStringList keys = reg.enum_Keys();
        foreach(QString key, keys){
            QVariantMap software;
            VRegistry subReg;
            subReg.OpenKey(HKEY_LOCAL_MACHINE, registry + key);
            if (!subReg.get_REG_SZ("DisplayName").isEmpty())
                software["DisplayName"] = subReg.get_REG_SZ("DisplayName");
            if (!subReg.get_REG_SZ("Publisher").isEmpty())
                software["Publisher"] = subReg.get_REG_SZ("Publisher");
            if (!subReg.get_REG_SZ("InstallLocation").isEmpty())
                software["InstallLocation"] = subReg.get_REG_SZ("InstallLocation");
            if (!subReg.get_REG_SZ("InstallDate").isEmpty())
                software["InstallDate"] = subReg.get_REG_SZ("InstallDate");
            if (!subReg.get_REG_SZ("URLInfoAbout").isEmpty())
                software["URLInfoAbout"] = subReg.get_REG_SZ("URLInfoAbout");
            if (!subReg.get_REG_EXPAND_SZ("UninstallString").isEmpty())
                software["UninstallString"] = subReg.get_REG_EXPAND_SZ("UninstallString");
            if (!subReg.get_REG_EXPAND_SZ("QuietUninstallString").isEmpty())
                software["QuietUninstallString"] = subReg.get_REG_EXPAND_SZ("QuietUninstallString");
            if (!subReg.get_REG_SZ("DisplayVersion").isEmpty())
                software["DisplayVersion"] = subReg.get_REG_SZ("DisplayVersion");

            softwaresJson[key] = QJsonObject::fromVariantMap(software);
        }
        qDebug() << softwaresJson;
    }
    return softwaresJson;
}

#elif defined(Q_OS_LINUX)
QJsonObject cacic_software::coletaLinux()
{

    OperatingSystem operatingSystem;

    if( operatingSystem.getIdOs() == OperatingSystem::LINUX_ARCH ) {
        return coletaArch();
    } else if ( operatingSystem.getIdOs() == OperatingSystem::LINUX_DEBIAN ||
                operatingSystem.getIdOs() == OperatingSystem::LINUX_UBUNTU ) {
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
        QJsonObject packageJson;

        QStringList packageInfo = console(QString("pacman -Qi ").append(packageName)).split("\n");
//            qDebug() << packageInfo;

        packageJson["name"] = QJsonValue::fromVariant(QString(packageName));
        foreach(QString line, packageInfo) {
            if(line.contains("Version"))
                packageJson["version"] = line.split(":")[1].mid(1);
            if(line.contains("Description"))
                packageJson["description"] = line.split(":")[1].mid(1);
            if(line.contains("URL")) {
                QStringList url = line.split(":");
                QString urlString;

                for(int i = 1 ; i < url.size() ; ++i){
                    urlString.append(url[i]);
                    if(i == 1 ) urlString.append(":");
                }

                packageJson["url"] = urlString.mid(1);
            }
            if(line.contains("Installed size"))
                packageJson["installed_size"] = line.split(":")[1].mid(1);
            if(line.contains("Install Date"))
                packageJson["install_date"] = line.split(":")[1].mid(1);
        }
        softwaresJson[packageName] = packageJson;
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
        QJsonObject packageJson;

        QStringList packageInfo = console(QString("apt-cache show ").append(packageName)).split("\n");
//            qDebug() << packageInfo;

        packageJson["name"] = QJsonValue::fromVariant(QString(packageName));
        foreach(QString line, packageInfo) {
            if(line.contains("Version:"))
                packageJson["version"] = line.split(":")[1].mid(1);
            if(line.contains("Description-en:"))
                packageJson["description"] = line.split(":")[1].mid(1);
            if(line.contains("Homepage:")) {
                QStringList url = line.split(":");
                QString urlString;

                for(int i = 1 ; i < url.size() ; ++i){
                    urlString.append(url[i]);
                    if(i == 1 ) urlString.append(":");
                }

                packageJson["url"] = urlString.mid(1);
            }
            if(line.contains("Installed-Size:"))
                packageJson["installed_size"] = line.split(":")[1].mid(1);
        }
        softwaresJson[packageName] = packageJson;
        int counterPackages = softwaresJson.size();
    }

    return softwaresJson;
}
#endif

QJsonObject cacic_software::toJsonObject()
{
    return coletaSoftware;
}
