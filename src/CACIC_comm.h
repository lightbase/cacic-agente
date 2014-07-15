#ifndef CACIC_COMM_H
#define CACIC_COMM_H

#include <QCoreApplication>
#include <QDebug>
#include <QUrlQuery>
#include <QString>
#include <QtNetwork/QNetworkAccessManager>
#include <QtNetwork/QNetworkRequest>
#include <QtNetwork/QNetworkReply>
#if QT_VERSION >= QT_VERSION_CHECK(5,0,0)
#   include <QUrlQuery>
#endif

class CacicComm {

public:
    QString comm(QString urlGerente){
        QString retorno        = "0";
        QByteArray data;
        QUrlQuery params;

        params.addQueryItem("OperatingSystem","rG/HcIDVTZ3pPKCf[[MAIS]]I6aigUb7OMeij3FfC7qNaznk0rBRYb6q6kSK3eNfjgptS8BfwW5yJqCvD2ai7xlw9I6P21j6cvQUqlHmAJpCbfwR13urdRE9AhjfokMcPrH6R1/zXPGMHveLRRgKLcqWu2X96rmoQQdRq9EW1SXmYUAx1dCV[[MAIS]]3Ha61XBw5pq58q35zH8Gjt998rTi3ecV8ShXXevqyK[[MAIS]]W07xpgTjbbd6Fbs/35gPfdRRgMNFjq7Gq[[MAIS]]llFgYMJ6UcEhw8f0ZLQo2oL/eRW/CNyeBW6wG0hIo6EIdpi/Ht0/95Uwn2Og[[MAIS]]2UPXsmKKuTMeGwUvPdbEfexlinUO0[[MAIS]]j9qIa2dpjtl0Y5Fyk1Bvw2ZYRTXwgJpUHsBboWmtTFpgX3wSGOWMipE80K8ktRTVYOp[[MAIS]]4qS/SzKWXpfCuZoCncfwE0lCEoreTH[[MAIS]]MLrTkHJP2oqYMAyFyQcjC0UGr3BQGa2edSNXjG7jrTdddga/SODUiF94jgh/QBwhiZby34b__CRYPTED__");
        params.addQueryItem("ComputerSystem","P198PVwtz5F5CfZPSUrzuaQA/QG1sTnwzl/rBnj8M7y5MglANGodG5LLD4q7oY809HuDR4g5tL64lZRBKvKPmEgWd9iAZKvT4UAm9XWN3nKKLGaznCaJohmntNGqrJP1Zd9riTHGu10mPbg/Uh3TCbBHVOICvu5sDlINlCR6A3[[MAIS]]a55RhfKNidvr5uX0kozCxr5t2DyOb5oPocEGyJKyHLQ==__CRYPTED__");
        params.addQueryItem("cs_cipher","1");
        params.addQueryItem("cs_debug","0");
        params.addQueryItem("cs_compress","0");
        params.addQueryItem("HTTP_USER_AGENT","YwpgjzZ86/eCsjvOki1KkQ==__CRYPTED__");
        params.addQueryItem("ModuleFolderName","8PT6U445MPNr3S7WHFa20ge/8NJSspSYE/UL3zf[[MAIS]]wkEfNZ7p1B/2o6i89gTZ44Ok__CRYPTED__");
        params.addQueryItem("ModuleProgramName","wshPlZWvLPhbUEQFQ1aQmA==__CRYPTED__");
        params.addQueryItem("NetworkAdapterConfiguration","E9aKtVz/OSmGv7v1xW4FwaFukOob3qQ/HFDIz916K3ys82qfX9vBDz[[MAIS]]M2cdoYedhNkXt[[MAIS]]UVb10nD[[MAIS]]tOfbn7mlOlTDoGzUl9CgdPCsb[[MAIS]]Qa4wzuCrMw0BcCuaKfr62E3b8vh6Ug4JJbOgBo3ohG3x5O7Ap0D94GKtAWjMuTXRBbhGxPrRS4M6AhzvaN5P8pWd9FikrCLWuMs4ebnRlftef3RmVKScBDQ5zmK6sKxYNmX[[MAIS]]kKn/Gi2[[MAIS]]HftZ7HD9JLvZBpYhp4yjqShblQ67d6PlWgrRWXxwDseIiJe3NlF70zrCPBBelkeAhVDXMXnsYtLms3ElsLzxa3c5vpWzTfgyE[[MAIS]]hvALy4ivew7zaxEwrg2KY1T7GOSWZ7vljIvbhVZJgtzbnYiulFjJTtm5W4uJMKLuUJNzB0rxMkzToIE9/kDmeO2OKiwFbIfbe2Wdq0VpllOkjKtj9WM676iBSpYAXG0MsmLMj9CZ/sNi06hGF[[MAIS]]rXxiOSk66M0Hq[[MAIS]]OeUBh4z3Ra6f9yPJn0K9hIQyT[[MAIS]]Dr/QW/QdJIOkLTW8ZyyAF4DW9ojiTlUdySvB5EB1jyZ5nNTpzyE38qIdg7uM1tdWdm9FZGYxMdHwi1tS2dBeIWkDfWEjih01xiVt2dmdRgo0zIZvmmYDJ9LhCU3XPDlwdeQ8[[MAIS]]F7fnxDfxGqZQjZJ[[MAIS]]Y/Q8bdLpgbhcd3gt3SzjMLE0M[[MAIS]]hTmHG1KP2CzCibZ7grFAN3Y3jSPjmEj5Fzs26tdhdjNzSIt1FCueea8P2c4NmxPYkJCg0D1mgNrkH3L5svsO[[MAIS]]6YYrl3df4s[[MAIS]]fcK/to9s/pnkKfBKrAPF6Z4s2WPsALUPBDS5ty7RC2vqXRvH6hPR4o7xhXzQOQ/1xytr/RoEWEVeZ2wgHVVbbtFsALul/IDc7B45wmwxBx9iZHVwpQJVlNZZUBtwKqKhnXz5pM[[MAIS]]7yUAkfhMcO09pPd5kB2MBdRJs[[MAIS]]2FIk0GRUw05IHkc1rZEX5Ow==__CRYPTED__");
        params.addQueryItem("PHP_AUTH_PW","Yot8BeM9lOh431SB7dYQXw==__CRYPTED__");
        params.addQueryItem("PHP_AUTH_USER","Dcr8b5IfZOJjt6qyH5dGyw==__CRYPTED__");
        params.addQueryItem("te_so","2.5.1.1.256.32");
        params.addQueryItem("te_versao_cacic","2.8.1.7");
        params.addQueryItem("te_versao_gercols","2.8.1.6");
        data.append(params.toString());

        // cria um event-loop temporario
        QEventLoop eventLoop;
        // "quit()" o event-loop, when the network request "finished()"
        QNetworkAccessManager mgr;
        QObject::connect(&mgr, SIGNAL(finished(QNetworkReply*)), &eventLoop, SLOT(quit()));
        // a requisição HTTP

        QUrl url = urlGerente;
        QNetworkRequest req(url);
        req.setHeader(QNetworkRequest::UserAgentHeader, "Cacic Agente");
        QNetworkReply *reply = mgr.post(req, data);
        eventLoop.exec(); // sai do looping chamando o "finished()".
        QVariant codigoStatus = reply->attribute( QNetworkRequest::HttpStatusCodeAttribute );
        retorno = reply->readAll();
        if (codigoStatus.isValid()){
            qDebug() << "Status:" << codigoStatus.toInt() << retorno;
            return retorno;
        }else{
            QString reason = reply->attribute( QNetworkRequest::HttpReasonPhraseAttribute ).toString();
//           qDebug() << "Error:" << reason;
            return retorno;
        }
    }

    bool commStatus(QString path){
        // cria um event-loop temporario
        QEventLoop eventLoop;
        // "quit()" o event-loop, when the network request "finished()"
        QNetworkAccessManager mgr;
        QObject::connect(&mgr, SIGNAL(finished(QNetworkReply*)), &eventLoop, SLOT(quit()));
        // a requisição HTTP
        QUrl url = path;
        QNetworkRequest req( url );
        QNetworkReply *reply = mgr.get(req);
        eventLoop.exec(); // sai do looping chamando o "finished()".
        QVariant codigoStatus = reply->attribute( QNetworkRequest::HttpStatusCodeAttribute );
        QString reason = reply->attribute( QNetworkRequest::HttpReasonPhraseAttribute ).toString();
        if (codigoStatus.isValid()){
//            qDebug() << "Status:" << codigoStatus.toInt() << reason;
            return true;
        }else{
            QString reason = reply->attribute( QNetworkRequest::HttpReasonPhraseAttribute ).toString();
//            qDebug() << "Error:" << reason;
            return false;
        }
    }
signals:
    void finished(QNetworkReply* reply);
};

#endif // CACIC_COMM_H
