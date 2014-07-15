#include "ccacic.h"

CCacic::CCacic(QObject *parent) :
    QObject(parent)
{
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





