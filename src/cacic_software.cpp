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

QJsonObject cacic_software::coletaWin()
{
    QJsonObject softwaresJson;
    QStringList regedit;
    regedit.append("HKEY_LOCAL_MACHINE\\SOFTWARE\\Wow6432Node\\Microsoft\\Windows\\CurrentVersion\\Uninstall");
    regedit.append("HKEY_LOCAL_MACHINE\\SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Uninstall");
    foreach(QString registry, regedit){
        QSettings softwares(registry, QSettings::NativeFormat);
    //    qDebug() << softwares.childGroups();
        foreach (QString group, softwares.childGroups()){
            QVariantMap software;
            softwares.beginGroup(group);
            qDebug() << softwares.fileName() << softwares.value("DisplayName");
            if (!softwares.value("DisplayName").isNull())
                software["DisplayName"] = softwares.value("DisplayName");
            if (!softwares.value("Publisher").isNull())
                software["Publisher"] = softwares.value("Publisher");
            if (!softwares.value("InstallLocation").isNull())
                software["InstallLocation"] = softwares.value("InstallLocation");
            if (!softwares.value("InstallDate").isNull())
                software["InstallDate"] = softwares.value("InstallDate");
            if (!softwares.value("URLInfoAbout").isNull())
                software["URLInfoAbout"] = softwares.value("URLInfoAbout");
            if (!softwares.value("UninstallString").isNull())
                software["UninstallString"] = software.value("UninstallString");
            if (!softwares.value("QuietUninstallString").isNull())
                software["QuietUninstallString"] = software.value("QuietUninstallString");
            softwares.endGroup();
            softwaresJson[group] = QJsonObject::fromVariantMap(software);
        }
//    qDebug() << softwaresJson;
    }
    return softwaresJson;
}

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

    QStringList packages = console("dpkg --get-selections").split("\n");

    foreach(QString package, packages) {
        QString packageName = package.split(" ")[0];
        QJsonObject packageJson;

        QStringList packageInfo = console(QString("apt-cache show ").append(packageName)).split("\n");
//            qDebug() << packageInfo;

        packageJson["name"] = QJsonValue::fromVariant(QString(packageName));
        foreach(QString line, packageInfo) {
            if(line.contains("Version"))
                packageJson["version"] = line.split(":")[1].mid(1);
            if(line.contains("Description"))
                packageJson["description"] = line.split(":")[1].mid(1);
            if(line.contains("Homepage")) {
                QStringList url = line.split(":");
                QString urlString;

                for(int i = 1 ; i < url.size() ; ++i){
                    urlString.append(url[i]);
                    if(i == 1 ) urlString.append(":");
                }

                packageJson["url"] = urlString.mid(1);
            }
            if(line.contains("Installed-Size"))
                packageJson["installed_size"] = line.split(":")[1].mid(1);
        }
        softwaresJson[packageName] = packageJson;
    }

    return softwaresJson;
}

QJsonObject cacic_software::toJsonObject()
{
    return coletaSoftware;
}
