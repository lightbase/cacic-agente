#ifndef UICLIENT_H
#define UICLIENT_H

#include <QAbstractSocket>
#include <QString>
#include <QTcpSocket>
#include <QWidget>
#include "../src/logcacic.h"
#include "../src/identificadores.h"

class UiClient: public QWidget
{
    Q_OBJECT
public:
    UiClient(const QString &dirpath,QWidget *parent =0);
    bool isConnected();

public slots:
    void on_forcarClicked();
    void on_finalizarClicked();

private slots:
    void on_bytesWritten(qint64 bytes);
    void on_connected();
    void on_disconnected();
    void on_readyRead();

private:
    void parseData(const QString &dataReceived);
    void setupSocketConnection();
//    void timeout2Ack(QByteArray &dataSent);

    bool connected;
    LogCacic *logcacic;
//    QByteArray timeoutBuffer;
    QString mainFolder;
    QTcpSocket *socket;
};

#endif // UICLIENT_H
