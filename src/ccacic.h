/*-----------------------------------------------------------------------------------------
 *
 * Project Cacic Agente
 *    File created by Lightbase
 *
 * Developers: Eric Menezes Noronha (eric.m.noronha@lightbase.com.br); GitHub: ericmenezes
 *             Thiago Rocha         (thiago.rocha@lightbase.com.br)  ;
 *
 *-----------------------------------------------------------------------------------------*/

#ifndef CCACIC_H
#define CCACIC_H

#include <../src/vregistry.h>
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
#include <servicecontroller.h>
#include <identificadores.h>

namespace CCacic {

//    QString getValueFromFile(QString sectionName, QString keyName, QString filePath);
//    void setValueToFile(QString sectionName, QString keyName, QString value, QString filePath);
//    QString getValueFromTags(QString fullString, QString tag, QString tagType = "[]");
//    QString enCrypt(std::string str_in, std::string iv);
//    QString deCrypt(std::string str_in, std::string iv);
    bool createFolder(QString path);
    bool deleteFolder(QString path);
    bool deleteFile(QString path);
//    QJsonValue jsonValueFromJsonString(QString json, QString key);
    bool setJsonToFile(QJsonObject json, QString filepath);
    QJsonObject getJsonFromFile(QString filepath);
    QString startProcess(QString pathprogram, bool wait, bool *ok, QStringList arguments = QStringList());
    void setValueToRegistry(QString organization, QString application, QVariantMap values);
    QVariant getValueFromRegistry(QString organization, QString application, QString key);
    void removeRegistry(QString organization, QString application);
    QString convertDouble(const double &number, const int &precision = 10);
    std::string genRandomString(const int &len = 32);
    bool Md5IsEqual(QVariant document01, QVariant document02);
    bool Md5IsEqual(QByteArray document01, QString document02);
    QString toMD5(QString string);
    bool verificarRoot();
    bool verificarCacicInstalado();
    QString padronizarData(QString data);
    QString getMesFromString(QString mes);
    void salvarVersao(QString modulo);
    bool findProc(const char *name);
#ifdef Q_OS_WIN
    bool changeCacicVersion();
#endif

};

#endif // CCACIC_H
