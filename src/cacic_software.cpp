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
    QSettings softwares("HKEY_LOCAL_MACHINE\\SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Uninstall", QSettings::NativeFormat);
//    qDebug() << softwares.childGroups();
    foreach (QString group, softwares.childGroups()){
        QVariantMap software;
        softwares.beginGroup(group);
        software["DisplayName"] = softwares.value("DisplayName");
        software["Publisher"] = softwares.value("Publisher");
        software["InstallLocation"] = softwares.value("InstallLocation");
        software["InstallDate"] = softwares.value("InstallDate");
        software["URLInfoAbout"] = softwares.value("URLInfoAbout");
        softwares.endGroup();
        softwaresJson[group] = QJsonObject::fromVariantMap(software);
    }
//    qDebug() << softwaresJson;
    return softwaresJson;
}

void cacic_software::coletaLinux()
{

}
QJsonObject cacic_software::toJsonObject()
{
    return coletaSoftware;
}
