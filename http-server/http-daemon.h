#ifndef HTTPDAEMON
#define HTTPDAEMON

#endif // HTTPDAEMON
#include <QObject>
#include <QTcpServer>
#include <QTcpSocket>
#include "../src/qtservice/src/QtServiceBase"
#include <QDateTime>
#include "../installcacic-standalone/commsa.h"

// HttpDaemon is the the class that implements the simple HTTP server.
// Source: http://doc.qt.digia.com/solutions/4/qtservice/qtservice-example-server.html
 class HttpDaemon : public QTcpServer
 {
     Q_OBJECT
 public:
     HttpDaemon(quint16 port, QObject* parent = 0)
         : QTcpServer(parent), disabled(false)
     {
         listen(QHostAddress::Any, port);
     }

     void incomingConnection(int socket);

     void pause();

     void resume();

 private slots:
     void readClient();
     void discardClient();
     QString processRoutes(const QString &rota);
     QString getInstalaHash();
     QString getDefaultRoute();
     QString getErro();
     QString getDownload();

 private:
     bool disabled;
 };
