#ifndef CCACIC_H
#define CCACIC_H

#include <QCoreApplication>
#include <QProcess>
#include <QObject>
#include <QFile>
#include <QSettings>
#include <QDir>
#include <QDebug>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonValue>
#include "../src/crypto++/include/aes.h"
#include "../src/crypto++/include/base64.h"
#include "../src/crypto++/include/modes.h"
//#include <filters.h>
//#include <hex.h>
//#include <sha.h>
//#include <md5.h>

class CCacic : public QObject
{
    Q_OBJECT
public:
    explicit CCacic(QObject *parent = 0);
    QString getValueFromFile(QString sectionName, QString keyName, QString filePath);
    void setValueToFile(QString sectionName, QString keyName, QString value, QString filePath);
    QString getValueFromTags(QString fullString, QString tag, QString tagType = "[]");
    QString enCrypt(std::string str_in, std::string iv);
    QString deCrypt(std::string str_in, std::string iv);
    bool createFolder(QString path);
    bool deleteFolder(QString path);
    bool deleteFile(QString path);
    QJsonValue jsonValueFromJsonString(QString json, QString key);
    bool setJsonToFile(QJsonObject json, QString filepath);
    QJsonObject getJsonFromFile(QString filepath);
    QString startProcess(QString pathprogram, bool wait, bool *ok, QStringList arguments = QStringList());
    void setValueToRegistry(QString organization, QString application, QVariantMap values);
    QVariant getValueFromRegistry(QString organization, QString application, QString key);
    void removeRegistry(QString organization, QString application);


//Geters/seters:

    QString getCacicMainFolder() const;
    void setCacicMainFolder(const QString &value);

    QString getMainModuleName() const;
    void setMainModuleName(const QString &value);

    QString getUrlGerente() const;
    void setUrlGerente(const QString &value);

    QString getGerColsInfFilePath() const;
    void setGerColsInfFilePath(const QString &value);

    QString getChksisInfFilePath() const;
    void setChksisInfFilePath(const QString &value);

    QString getChaveCrypt() const;
    void setChaveCrypt(const QString &value);

private:
    QString cacicMainFolder;
    QString mainModuleName;
    QString urlGerente;
    QString gerColsInfFilePath;
    QString chksisInfFilePath;
    QString chaveCrypt;

signals:

public slots:

};

#endif // CCACIC_H
