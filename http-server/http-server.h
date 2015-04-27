#ifndef HTTPSERVER
#define HTTPSERVER

#include <../src/qtservice/src/QtServiceBase>
#include <QCoreApplication>
#include "http-daemon.h"
#include <QSettings>
#include <QDir>

// Source: http://doc.qt.digia.com/solutions/4/qtservice/qtservice-example-server.html
class HttpService : public QtService<QCoreApplication>
 {
 public:
     HttpService(int &argc, char **argv)
         : QtService<QCoreApplication>(argc, argv, "Qt HTTP Daemon")
     {
         setServiceDescription("A dummy HTTP service implemented with Qt");
         setServiceFlags(QtServiceBase::CanBeSuspended);

         // Cria arquivo de configuração
         QString filename = QDir::tempPath().toLatin1().constData();
         filename += "/QtSoftware.conf";
         QFile file(filename);
         if (file.open(QIODevice::ReadWrite)) {
             QTextStream stream(&file);
             stream << "" << endl;
         }
     }

protected:
    void start();

    void pause();

    void resume();

private:
   HttpDaemon *daemon;

};

#endif // HTTPSERVER
