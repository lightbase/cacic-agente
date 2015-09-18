#ifndef SERVERTHREAD_H
#define SERVERTHREAD_H

#include <QString>
#include <QThread>
#include <QTcpSocket>
#include "../src/logcacic.h"
#include "../src/identificadores.h"

class ServerThread : public QThread
{
    Q_OBJECT
public:
    explicit ServerThread(const int &sockDescriptor, const QString &dir, QObject *parent = 0);
    void run();

signals:

private slots:
    void readyRead();
    void disconnected();

private:
    int socketDescriptor;
    LogCacic *logcacic;
    QString cacicMainFolder;
    QTcpSocket *socket;

};

#endif // SERVERTHREAD_H
