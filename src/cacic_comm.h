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
    CacicComm (const QString &urlGerente,          const QString &operatingSystem,     const QString &computerSystem,  const QString &csCipher,
               const QString &csDebug,             const QString &csCompress,          const QString &httpUserAgent,   const QString &moduleFolderName,
               const QString &moduleProgramName,   const QString &networkConfiguration,const QString &phpAuthPw,       const QString &phpAuthUser,
               const QString &so,                  const QString &cacicVersion,        const QString &gercolsVersion);
    QJsonObject comm(QString route, bool *ok, const QJsonObject &json = QJsonObject(), bool isSsl = false);
    bool commStatus();
    QJsonObject login(bool *ok);
    bool ftpDownload(const QString &path, const QString &pathDownload);
    bool ftpDownload(const QString &urlServer, const QString &path, const QString &pathDownload);
    bool httpDownload(const QString &path, const QString &pathDownload);
    bool httpDownload(const QString &urlServer, const QString &path,const QString &pathDownload);
    bool fileDownload(const QString &mode, const QString &path, const QString &pathDownload);
    bool fileDownload(const QString &mode, const QString &urlServer, const QString &path, const QString &pathDownload);

    QString getUrlSsl ();
    void setUrlSsl(const QString &value);
    QString getUrlGerente ();
    void setUrlGerente(QString value);
    QString getPassword();
    void setPassword(const QString &value);
    QString getUsuario();
    void setUsuario(const QString &value);
    QString getFtpUser() const;
    void setFtpUser(const QString &value);
    QString getFtpPass() const;
    void setFtpPass(const QString &value);

signals:
    void quitLoop();
//    void fileDownloadFinished();

private slots:

    bool startRequest(QUrl url);
    void fileDownloadFinished();
    void fileDownloadReadyRead();
private:

    QUrlQuery params;
    QString urlGerente;
    QString urlSsl;
    QString usuario;
    QString password;
    QString session;
    QString ftpUser;
    QString ftpPass;

    QFile *fileHandler;
    QNetworkReply *reply;

};
#endif // CACIC_COMM_H
