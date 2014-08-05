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
    // FIXME: Get from configuration
    QString usuario = "user";
    QString password = "123456";

public:
    CacicComm (){

    }

    CacicComm (QString urlGerente,          QString operatingSystem,     QString computerSystem,  QString csCipher,
               QString csDebug,             QString csCompress,          QString httpUserAgent,   QString moduleFolderName,
               QString moduleProgramName,   QString networkConfiguration,QString phpAuthPw,       QString phpAuthUser,
               QString so,                  QString cacicVersion,        QString gercolsVersion)
    {
        this->urlGerente = urlGerente;

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

    QJsonObject comm(QString route, const QJsonObject &json)
    {
        QString retorno        = "0";
        QByteArray data;
        QUrl url = urlGerente + route;

        //data.append(params.toString());

        // Add JSON
        QJsonDocument d(json);
        data.append(d.toVariant().toString());

        // cria um event-loop temporario
        QEventLoop eventLoop;
        // "quit()" o event-loop, when the network request "finished()"
        QNetworkAccessManager mgr;
        QObject::connect(&mgr, SIGNAL(finished(QNetworkReply*)), &eventLoop, SLOT(quit()));

        // a requisição HTTP
        QNetworkRequest req(url);
        req.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");
        req.setHeader(QNetworkRequest::LocationHeader, "Cacic Agente");

        QNetworkReply *reply = mgr.post(req, data);
        eventLoop.exec(); // sai do looping chamando o "finished()".

        if (reply->error() == QNetworkReply::NoError) {

            QString strReply = (QString)reply->readAll();

            //parse json
            qDebug() << "Response:" << strReply;
            QJsonDocument jsonResponse = QJsonDocument::fromJson(strReply.toUtf8());

            QJsonObject jsonObj = jsonResponse.object();

            //qDebug() << "Time:" << jsonObj["time"].toString();
            //qDebug() << "Date:" << jsonObj["date"].toString();

            delete reply;

            return jsonObj;
        } else {
            //failure
            qDebug() << "Failure" <<reply->errorString();
            delete reply;

            return QJsonObject ();
        }
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
        QUrl url = urlGerente;
        QNetworkRequest req( url );
        QNetworkReply *reply = mgr.get(req);
        eventLoop.exec(); // sai do looping chamando o "finished()".

        QString reason = reply->attribute( QNetworkRequest::HttpReasonPhraseAttribute ).toString();
        if (reply->attribute( QNetworkRequest::HttpStatusCodeAttribute ).isValid()){
            //            qDebug() << "Status:" << codigoStatus.toInt() << reason;
            return true;
        }else{
            reason = reply->attribute( QNetworkRequest::HttpReasonPhraseAttribute ).toString();
            //            qDebug() << "Error:" << reason;
            return false;
        }
    }

    QJsonObject login() {
        // Cria dados de login
        QVariantMap loginMap;
        loginMap["user"] = this->usuario;
        loginMap["password"] = this->password;
        QJsonObject loginJson = QJsonObject::fromVariantMap(loginMap);

        // Cria conexão e retorna Json da sessão
        QJsonObject retorno = this->comm("login", loginJson);

        return retorno;

    }

signals:
    void finished(QNetworkReply* reply);
};

#endif // CACIC_COMM_H
