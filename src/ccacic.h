#ifndef CCACIC_H
#define CCACIC_H

#include <QCoreApplication>
#include <QProcess>
#include <QObject>
#include <QFile>
#include <QSettings>
#include <QDir>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonValue>
#include <sstream>
#include <iostream>
#include <string>
//#include "../src/crypto++/include/aes.h"
//#include "../src/crypto++/include/base64.h"
//#include "../src/crypto++/include/modes.h"
#include <QCryptographicHash>
#include <console.h>
#include "servicecontroller.h"
#include "identificadores.h"

class CCacic
{
public:
    CCacic();
//    QString getValueFromFile(QString sectionName, QString keyName, QString filePath);
//    void setValueToFile(QString sectionName, QString keyName, QString value, QString filePath);
//    QString getValueFromTags(QString fullString, QString tag, QString tagType = "[]");
//    QString enCrypt(std::string str_in, std::string iv);
//    QString deCrypt(std::string str_in, std::string iv);
    static bool createFolder(QString path);
    static bool deleteFolder(QString path);
    static bool deleteFile(QString path);
//    QJsonValue jsonValueFromJsonString(QString json, QString key);
    static bool setJsonToFile(QJsonObject json, QString filepath);
    static QJsonObject getJsonFromFile(QString filepath);
    static QString startProcess(QString pathprogram, bool wait, bool *ok, QStringList arguments = QStringList());
    static void setValueToRegistry(QString organization, QString application, QVariantMap values);
    static QVariant getValueFromRegistry(QString organization, QString application, QString key);
    static void removeRegistry(QString organization, QString application);
    static QString convertDouble(const double &number, const int &precision = 10);
    static std::string genRandomString(const int &len = 32);
    static bool Md5IsEqual(QVariant document01, QVariant document02);
    static bool Md5IsEqual(QByteArray document01, QString document02);
    static QString toMD5(QString string);
    static bool verificarRoot();
    static bool verificarCacicInstalado();
    static QString padronizarData(QString data);
    static QString getMesFromString(QString mes);
    static void salvarVersao(QString modulo);

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
};

#endif // CCACIC_H
