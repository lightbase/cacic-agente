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

class CacicComm {

private:
    QUrlQuery params;
    QString urlGerente;
    QString urlSsl;
    QString usuario;
    QString password;
    QString session;

public:
    CacicComm (){
    }

    CacicComm (QString urlGerente,          QString operatingSystem,     QString computerSystem,  QString csCipher,
               QString csDebug,             QString csCompress,          QString httpUserAgent,   QString moduleFolderName,
               QString moduleProgramName,   QString networkConfiguration,QString phpAuthPw,       QString phpAuthUser,
               QString so,                  QString cacicVersion,        QString gercolsVersion)
    {
        this->setUrlGerente(urlGerente);
        params.addQueryItem("OperatingSystem", operatingSystem);
        params.addQueryItem("ComputerSystem",computerSystem);
        params.addQueryItem("cs_cipher",csCipher);
        params.addQueryItem("cs_debug",csDebug);
        params.addQueryItem("cs_compress",csCompress);
        params.addQueryItem("HTTP_USER_AGENT",httpUserAgent);
        params.addQueryItem("ModuleFolderName",moduleFolderName);
        params.addQueryItem("ModuleProgramName",moduleProgramName);
        params.addQueryItem("NetworkAdapterConfiguration",networkConfiguration);
        params.addQueryItem("PHP_AUTH_PW",phpAuthPw);
        params.addQueryItem("PHP_AUTH_USER",phpAuthUser);
        params.addQueryItem("te_so",so);
        params.addQueryItem("te_versao_cacic",cacicVersion);
        params.addQueryItem("te_versao_gercols",gercolsVersion);


    }

    QJsonObject comm(QString route, bool *ok, const QJsonObject &json = QJsonObject(), bool isSsl = false)
    {
        *ok = false;
        QByteArray data;
        QNetworkRequest req;
        QUrl url;
        QString strReply;
        QJsonObject jsonObj;
        if (isSsl){
            url = urlSsl.isEmpty() ? "https://" + this->urlGerente + route : this->urlSsl + route;
            if (!url.isValid()){
                jsonObj["error"] = QJsonValue::fromVariant(QVariant("Invalid Url"));
                return jsonObj;
            }
            req.setSslConfiguration(QSslConfiguration::defaultConfiguration());
        } else
            url = "http://" + urlGerente + route;
        req.setUrl(url);
        req.setHeader(QNetworkRequest::LocationHeader, "Cacic Agente");
        if (json.empty()){
            //se não for passado QJson, será mandado os valores do get/test antigo. (Será retirado depois)
            req.setHeader(QNetworkRequest::ContentTypeHeader, "application/x-www-form-urlencoded");
            data.append(params.toString());
        } else {
            // Add JSON
            QJsonDocument d(json);
            data.append(d.toJson());
            req.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");
        }
        QEventLoop eventLoop;
        QNetworkAccessManager mgr;
        QObject::connect(&mgr, SIGNAL(finished(QNetworkReply*)), &eventLoop, SLOT(quit()));

        QNetworkReply *reply = mgr.post(req, data);
        if (!reply->sslConfiguration().isNull()){
            reply->ignoreSslErrors();
        }
        eventLoop.exec(); // sai do looping chamando o "finished()".

        jsonObj.insert("codestatus", QJsonValue::fromVariant(reply->attribute(QNetworkRequest::HttpStatusCodeAttribute)));

        if (reply->error() == QNetworkReply::NoError) {
            //se não houver erro, grava o retorno;
            QVariant replyVariant = reply->readAll();
            QJsonDocument replyDocument = QJsonDocument::fromJson(replyVariant.toByteArray());
            jsonObj["reply"] = (!replyDocument.isNull()) ?
                                replyDocument.object() :
                                QJsonValue::fromVariant(replyVariant.toString());
            *ok = true;
            delete reply;
        } else {
            strReply = reply->errorString();
            jsonObj.insert("error", QJsonValue::fromVariant(strReply));
            delete reply;
        }
        return jsonObj;
    }


    /* commStatus
     * execulta um teste do tipo GET na urlGerente;
     * @return retorna o resultado da conexão HTTP:
     *      exemplo: 200 OK
     */
    bool commStatus(){
        // cria um event-loop temporario
        QEventLoop eventLoop;
        // "quit()" o event-loop, when the network request "finished()"
        QNetworkAccessManager mgr;
        QObject::connect(&mgr, SIGNAL(finished(QNetworkReply*)), &eventLoop, SLOT(quit()));
        // a requisição HTTP
        QUrl url = "http://" + urlGerente;
        QNetworkRequest req( url );
        req.setHeader(QNetworkRequest::LocationHeader, "Cacic Agente");
        QNetworkReply *reply = mgr.get(req);
        eventLoop.exec(); // sai do looping chamando o "finished()".

        QString reason = reply->attribute( QNetworkRequest::HttpReasonPhraseAttribute ).toString();
        if (reply->attribute( QNetworkRequest::HttpStatusCodeAttribute ).isValid()){
//            qDebug() << "Status:" << reply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt() << reason;
            return true;
        }else{
            reason = reply->attribute( QNetworkRequest::HttpReasonPhraseAttribute ).toString();
//              qDebug() << "Error:" << reason;
            return false;
        }
    }

    /* login
     * realiza login com usuario e senha
     * @return retorna json com sessão e chave de criptografia
     */
    QJsonObject login(bool *ok) {
        *ok = false;
        // Cria dados de login
        QVariantMap login;
        login["user"] = this->usuario;
        login["password"] = this->password;
        // Cria conexão e retorna Json da sessão
        QJsonObject retorno = this->comm("/ws/neo/getLogin", ok, QJsonObject::fromVariantMap(login), true);
        if (*ok)
            this->session = retorno["reply"].toObject()["session"].toString();
        return retorno;
    }

    bool ftpDownload(QString path){
        QUrl url(urlGerente);
        url.setScheme("ftp");
        url.setPath(path);

    }

    QString getUrlSsl (){
        return this->urlSsl;
    }

    void setUrlSsl(QString value){
        this->urlSsl = value;
    }

    QString getUrlGerente (){
        return this->urlGerente;
    }

    void setUrlGerente(QString value){
        if (value.contains("://", Qt::CaseInsensitive)){
            value = value.mid(value.indexOf("://") + 3);
        }
        if (value.endsWith("/")){
            value.remove(value.size()-1, 1);
        }
        this->urlGerente = value;
    }

    QString getPassword()
    {
        return this->password;
    }

    void setPassword(QString value)
    {
        this->password = value;
    }
    QString getUsuario()
    {
        return this->usuario;
    }

    void setUsuario(QString value)
    {
        this->usuario = value;
    }


signals:
    void finished(QNetworkReply* reply);

};
#endif // CACIC_COMM_H
