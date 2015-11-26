#ifndef SERVICECLIENT_H
#define SERVICECLIENT_H

#include <QAbstractSocket>
#include <QString>
#include <QTimer>
#include <QTcpSocket>
#include <QObject>
#include "../src/logcacic.h"
#include "../src/identificadores.h"
#include "../src/workerthread.h"

class ServiceClient: public QObject
{
    Q_OBJECT
public:
    static ServiceClient *Instance(const QString &dirpath, QObject *parent = 0);
    bool isConnected();

signals:
    void finalizar();
#ifdef Q_OS_WIN
    void uiStopped();
#endif
public slots:
#ifdef Q_OS_WIN
    void on_sendStopUiMsg();
#endif

private slots:
    void on_bytesWritten(qint64 bytes);
    void on_connected();
    void on_disconnected();
    void on_readyRead();
    void setupSocketConnection();

private:
    ServiceClient(QObject *parent = 0);
    ServiceClient(const QString &dirpath,QObject *parent = 0);
    ServiceClient(ServiceClient const &ref, QObject *parent = 0);
    ServiceClient operator=(ServiceClient const &ref);

    QByteArray formatData(QString message, int messageLength);
    void parseData(const QString &dataReceived);

    bool connected;
    bool canSend;
    int connectionTrials = 0;
    QByteArray lastDataWritten;
    LogCacic *logcacic;
    QString mainFolder;
    QTimer *connectTimer;
    QTimer *reconnectTimer;
    QTcpSocket *socket;

    static int const CONNECTION_TIMEOUT;
    static int const CONNECT_TIMER_TIMEOUT;
    static int const MAX_CONNECTION_TRIALS;
    static int const MAX_MSGBUFFER_SIZE;
    static int const RECONNECT_TIMER_TIMEOUT;

    static WorkerThread *workerThread;
    static ServiceClient *pInstance;
};

#endif // SERVICECLIENT_H
