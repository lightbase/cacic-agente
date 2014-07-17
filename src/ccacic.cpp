#include "ccacic.h"

CCacic::CCacic(QObject *parent) :
    QObject(parent)
{
}

QString CCacic::getValueFromFile(QString sectionName, QString keyName, QString filePath)
{
    QFile file(filePath);
    sectionName = "<" + sectionName + ">";
    keyName     = keyName + "=";
    int sizeKeyName = keyName.size();

    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
       return "0";

    QString line = file.readLine();
    while (!file.atEnd()) {
       if (line.contains(sectionName, Qt::CaseInsensitive)) {
           line = file.readLine();
           while (!file.atEnd()){
               if (line.contains(keyName, Qt::CaseInsensitive)) {
                   file.close();
                   return line.mid(sizeKeyName).trimmed();
               }
           }
       } else {
           line = file.readLine();
       }
    }
    file.close();
    return "";
}

QString CCacic::getValueFromTags(QString fullString, QString tag, QString tagType) {
    QString tagFim = tagType.mid(0,1) + "/" + tag + tagType.mid(1);
    int tagSize;
    tag = tagType.mid(0,1) + tag + tagType.mid(1);
    tagSize = tag.size();
    return fullString.mid(fullString.indexOf(tag) + tagSize,
                          fullString.indexOf(tagFim) - (fullString.indexOf(tag) + tagSize)).trimmed();
}

/*Getters/Setters
 * Begin:
 */
QString CCacic::getCacicMainFolder() const
{
    return cacicMainFolder;
}

void CCacic::setCacicMainFolder(const QString &value)
{
    cacicMainFolder = value;
}
QString CCacic::getMainModuleName() const
{
    return mainModuleName;
}

void CCacic::setMainModuleName(const QString &value)
{
    mainModuleName = value;
}
QString CCacic::getUrlGerente() const
{
    return urlGerente;
}

void CCacic::setUrlGerente(const QString &value)
{
    urlGerente = value;
}
QString CCacic::getGerColsInfFilePath() const
{
    return gerColsInfFilePath;
}

void CCacic::setGerColsInfFilePath(const QString &value)
{
    gerColsInfFilePath = value;
}
QString CCacic::getChksisInfFilePath() const
{
    return chksisInfFilePath;
}

void CCacic::setChksisInfFilePath(const QString &value)
{
    chksisInfFilePath = value;
}

/*Getters/Setters
 * End.
 */





