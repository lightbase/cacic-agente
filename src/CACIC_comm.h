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


class CacicComm {

private:
    QUrlQuery params;
    QString urlGerente;

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

    QString comm(QString route)
    {
        QString retorno        = "0";
        QByteArray data;
        QUrl url = urlGerente + route;

        data.append(params.toString());

        // cria um event-loop temporario
        QEventLoop eventLoop;
        // "quit()" o event-loop, when the network request "finished()"
        QNetworkAccessManager mgr;
        QObject::connect(&mgr, SIGNAL(finished(QNetworkReply*)), &eventLoop, SLOT(quit()));

        // a requisição HTTP

        QNetworkRequest req(url);
        req.setHeader(QNetworkRequest::LocationHeader, "Cacic Agente");

        QNetworkReply *reply = mgr.post(req, data);
        eventLoop.exec(); // sai do looping chamando o "finished()".


        if (reply->attribute( QNetworkRequest::HttpStatusCodeAttribute ).isValid()){
            retorno = reply->readAll();
//            qDebug() << "Status:" << codigoStatus.toInt() << retorno;
        }else{
            retorno = reply->attribute( QNetworkRequest::HttpReasonPhraseAttribute ).toString();
//           qDebug() << "Error:" << reason;
        }
        return retorno;
    }

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

signals:
    void finished(QNetworkReply* reply);
};

#endif // CACIC_COMM_H
