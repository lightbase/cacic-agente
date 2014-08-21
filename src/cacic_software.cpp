#include "cacic_software.h"

cacic_software::cacic_software()
{
}

void cacic_software::iniciaColeta()
{
#ifdef Q_OS_WIN
    this->coletaSoftware = coletaWin();
#else
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

void cacic_software::coletaLinux()
{

}
QJsonObject cacic_software::toJsonObject()
{
    return coletaSoftware;
}
