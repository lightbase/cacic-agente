#ifndef CACIC_COMM_H
#define CACIC_COMM_H

#include <QCoreApplication>
#include <QObject>
#include <QDebug>
#include <QFile>
#include <QUrlQuery>
#include <QtNetwork/QNetworkAccessManager>
#include <QtNetwork/QNetworkRequest>
#include <QtNetwork/QNetworkReply>
#include <QUrlQuery>
#include <QJsonObject>
#include <QJsonDocument>

class CacicComm : public QObject{

    Q_OBJECT

public:

    CacicComm ();
    CacicComm (QString urlGerente,          QString operatingSystem,     QString computerSystem,  QString csCipher,
               QString csDebug,             QString csCompress,          QString httpUserAgent,   QString moduleFolderName,
               QString moduleProgramName,   QString networkConfiguration,QString phpAuthPw,       QString phpAuthUser,
               QString so,                  QString cacicVersion,        QString gercolsVersion);
    QJsonObject comm(QString route, bool *ok, const QJsonObject &json = QJsonObject(), bool isSsl = false);
    bool commStatus();
    QJsonObject login(bool *ok);
    bool ftpDownload(const QString &path);
    bool ftpDownload(const QString &urlServer, const QString &path);
    bool httpDownload(const QString &path);
    bool httpDownload(const QString &urlServer, const QString &path);
    QString getUrlSsl ();
    void setUrlSsl(QString value);
    QString getUrlGerente ();
    void setUrlGerente(QString value);
    QString getPassword();
    void setPassword(QString value);
    QString getUsuario();
    void setUsuario(QString value);



signals:
//    void finished(QNetworkReply* reply);

private slots:

    void fileDownloadFinished(QNetworkReply* reply);

private:

    bool fileDownload(const QString &mode, const QString &path);
    bool fileDownload(const QString &mode, const QString &urlServer, const QString &path);

    QUrlQuery params;
    QString urlGerente;
    QString urlSsl;
    QString usuario;
    QString password;
    QString session;

    QFile *fileHandler;

};
#endif // CACIC_COMM_H
