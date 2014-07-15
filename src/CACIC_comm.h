#ifndef CACIC_COMM_H
#define CACIC_COMM_H

#include <QCoreApplication>
#include <QDebug>
#include <QUrlQuery>
#include <QString>
#include <QtNetwork/QNetworkAccessManager>
#include <QtNetwork/QNetworkRequest>
#include <QtNetwork/QNetworkReply>
#include <QUrl>

class CacicComm {
public:

QString comm(QString urlGerente, QString request, QString localFolderName){
        //    QString retorno        = '0',
        //            gerColsInfPath = localFolderName + "/gercols.inf",
        //            chksisInfPath  = localFolderName + "/chksis.inf";
        //implementar
        return "";
    }

    bool commStatus200(){
        // cria um event-loop temporario
        QEventLoop eventLoop;
        // "quit()" o event-loop, when the network request "finished()"
        QNetworkAccessManager mgr;
        QObject::connect(&mgr, SIGNAL(finished(QNetworkReply*)), &eventLoop, SLOT(quit()));
        // a requisição HTTP
        QNetworkRequest req( QUrl( QString("http://www.google.com.br") ) );
        QNetworkReply *reply = mgr.get(req);
        eventLoop.exec(); // sai do looping chamando o "finished()".
        QVariant codigoStatus = reply->attribute( QNetworkRequest::HttpStatusCodeAttribute );
        QString reason = reply->attribute( QNetworkRequest::HttpReasonPhraseAttribute ).toString();
        if (codigoStatus.isValid()){
            qDebug() << "Status:" << codigoStatus.toInt() << reason;
            return true;
        }else{
            QString reason = reply->attribute( QNetworkRequest::HttpReasonPhraseAttribute ).toString();
            qDebug() << "Error: Off" << reason;
            return false;
        }
    }

};

#endif // CACIC_COMM_H
