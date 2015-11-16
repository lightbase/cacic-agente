#ifndef UISERVERTHREAD_H
#define UISERVERTHREAD_H

#include <QString>
#include <QThread>
#include <QTcpSocket>
#include "../src/logcacic.h"
#include "../src/identificadores.h"

class UiServerThread : public QThread
{
    Q_OBJECT
public:
    explicit UiServerThread(const int &sockDescriptor, const QString &dir, QObject *parent = 0);
    void run();

signals:
    void finalizarUi();

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

#endif // UISERVERTHREAD_H
