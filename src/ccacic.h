#ifndef CCACIC_H
#define CCACIC_H
#include <QCoreApplication>
#include <QObject>
#include <QDebug>
#include <QFile>
#include <iostream>
#include <iomanip>
#include <crypto++/aes.h>
#include <crypto++/base64.h>
#include <crypto++/modes.h>
#include <crypto++/filters.h>
#include <crypto++/hex.h>
#include <crypto++/sha.h>
#include <crypto++/md5.h>

class CCacic : public QObject
{
    Q_OBJECT
public:
    explicit CCacic(QObject *parent = 0);
    QString getValueFromFile(QString sectionName, QString keyName, QString filePath);
    QString getValueFromTags(QString fullString, QString tag, QString tagType = "[]");
    std::string enCrypt(QString str_in, QString key, QString iv);
    std::string deCrypt(QString str_in, QString key, QString iv);

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

private:
    QString cacicMainFolder;
    QString mainModuleName;
    QString urlGerente;
    QString gerColsInfFilePath;
    QString chksisInfFilePath;

signals:

public slots:

};

#endif // CCACIC_H
