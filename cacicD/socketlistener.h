#ifndef SOCKETLISTENER_H
#define SOCKETLISTENER_H

#include <QObject>
#include <QLocalServer>
#include <QLocalSocket>
#include "logcacic.h"
#include "identificadores.h"
#include "cacicthread.h"
#include "ccacic.h"
#include "cacic_comm.h"

class SocketListener : public QObject
{
    Q_OBJECT
public:
    explicit SocketListener(QString DirPath, QObject *parent = 0);
    ~SocketListener();

signals:
    void forcaColeta();
public slots:
    void newConnection();

private:
    QLocalServer *server;
    LogCacic *logcacic;
    QString applicationDirPath;
    CCacic *ccacic;
    void iniciarColetaForcada();
};

#endif // SOCKETLISTENER_H
