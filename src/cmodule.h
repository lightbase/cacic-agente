#ifndef CMODULE_H
#define CMODULE_H

#include <QString>
#include <QJsonObject>
#include <QJsonValue>
#include <QDebug>

class CModule
{
public:
    /**
     * @brief The TypeDownload enum
     */
    enum TypeDownload {
        Http,
        Ftp
    };
    /**
     * @brief The ErrorCodes enum
     */
    enum ErrorCodes{
        Success     =  0,
        EmptyJson   = -1,
        InvalidName = -2,
        InvalidHash = -3
    };

    CModule();
    /**
     * @brief loadFromJsonObject
     * @param value
     * @return ErrorCodes
     */
    int loadFromJsonObject(const QJsonObject &value);

    QString name() const;
    void setName(const QString &name);

    QString hash() const;
    void setHash(const QString &hash);

    QString urlDownload() const;
    void setUrlDownload(const QString &urlDownload);

    TypeDownload typeDownload() const;
    void setTypeDownload(const TypeDownload &typeDownload);

    QString version() const;
    void setVersion(const QString &version);

private:
    /**
     * @brief _name
     */
    QString _name;
    /**
     * @brief _hash
     */
    QString _hash;
    /**
     * @brief _urlDownload
     */
    QString _urlDownload;
    /**
     * @brief _version
     */
    QString _version;
    /**
     * @brief _typeDownload
     */
    TypeDownload _typeDownload;

};

#endif // CMODULE_H
