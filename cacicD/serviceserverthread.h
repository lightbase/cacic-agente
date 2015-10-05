#ifndef SERVERTHREAD_H
#define SERVERTHREAD_H

#include <QString>
#include <QThread>
#include <QTcpSocket>
#include "../src/logcacic.h"
#include "../src/identificadores.h"

class ServiceServerThread : public QThread
{
    Q_OBJECT
public:
    explicit ServiceServerThread(const int &sockDescriptor, const QString &dir, QObject *parent = 0);
    void run();

signals:
    void forcarColeta();
    void finalizarCacic();

private slots:
    void readyRead();
    void disconnected();

private:
    void parseData(const QString &dataReceived);
    void sendAck();

    int socketDescriptor;
    LogCacic *logcacic;
    QString cacicMainFolder;
    QTcpSocket *socket;

};

#endif // SERVERTHREAD_H
