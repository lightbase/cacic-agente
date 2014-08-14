#include "operatingsystem.h"

OperatingSystem::OperatingSystem()
{
    this->idOs = this->coletaIdOs();
    this->nomeOs = this->coletaNomeOs();
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
int OperatingSystem::coletaIdOs(){
#if defined (Q_OS_WIN)
    return QSysInfo::WindowsVersion;
#elif defined (Q_OS_LINUX)
    return LINUX_ARCH;
#else
    return 0;
#endif
}

QString OperatingSystem::coletaNomeOs()
{
    QString text;
    QStringList environment = QProcessEnvironment::systemEnvironment().toStringList();
    foreach (text, environment) {
        if (text.contains("OS="    , Qt::CaseInsensitive) ||
            text.contains("SESSION=", Qt::CaseInsensitive) ){
            QStringList split = text.split("=");
  //          qDebug() << split[1];
            return split[1];
          }
      }
      return "";
}

QJsonObject OperatingSystem::toJsonObject()
{
    QJsonObject json;
    json["idOs"] = QJsonValue::fromVariant(QVariant::fromValue(this->idOs));
    json["nomeOs"] = QJsonValue::fromVariant(this->nomeOs);
//    qDebug() << json;
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
