#ifndef UICLIENT_H
#define UICLIENT_H

#include <QAbstractSocket>
#include <QString>
#include <QTimer>
#include <QTcpSocket>
#include <QWidget>
#include "../src/logcacic.h"
#include "../src/identificadores.h"
#include "../src/workerthread.h"

class UiClient: public QWidget
{
    Q_OBJECT
public:
    static UiClient *Instance(const QString &dirpath, QWidget *parent = 0);
    bool isConnected();

signals:
    void finalizar();
public slots:
    void on_forcarClicked();
    void on_finalizarClicked();

private slots:
    void on_bytesWritten(qint64 bytes);
    void on_connected();
    void on_disconnected();
    void on_readyRead();
    void setupSocketConnection();

private:
    UiClient(QWidget *parent = 0);
    UiClient(const QString &dirpath,QWidget *parent =0);
    UiClient(UiClient const &ref, QWidget *parent = 0);
    UiClient operator=(UiClient const &ref);

    QByteArray formatData(QString message, int messageLength);
    void parseData(const QString &dataReceived);

    bool connected;
    bool canSend;
    int connectionTrials = 0;
    LogCacic *logcacic;
    QByteArray lastDataWritten;
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
    static UiClient *pInstance;
};

#endif // UICLIENT_H
