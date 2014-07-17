#ifndef CCACIC_H
#define CCACIC_H

#include <QCoreApplication>
#include <QObject>
#include <QDebug>
#include <QFile>
#include <QSettings>
#include <QDir>

class CCacic : public QObject
{
    Q_OBJECT
public:
    explicit CCacic(QObject *parent = 0);
    QString getValueFromFile(QString sectionName, QString keyName, QString filePath);
    void setValueToFile(QString sectionName, QString keyName, QString value, QString filePath);
    QString getValueFromTags(QString fullString, QString tag, QString tagType = "[]");
    bool createFolder(QString path);
    bool deleteFolder(QString path);
    bool deleteFile(QString path);

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
