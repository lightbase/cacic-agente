#include <http-daemon.h>

void HttpDaemon::incomingConnection(int socket) {
    if (disabled)
        return;

    // When a new client connects, the server constructs a QTcpSocket and all
    // communication with the client is done over this QTcpSocket. QTcpSocket
    // works asynchronously, this means that all the communication is done
    // in the two slots readClient() and discardClient().
    QTcpSocket* s = new QTcpSocket(this);
    connect(s, SIGNAL(readyRead()), this, SLOT(readClient()));
    connect(s, SIGNAL(disconnected()), this, SLOT(discardClient()));
    s->setSocketDescriptor(socket);

    //QtServiceBase::instance()->logMessage("New Connection");
}

void HttpDaemon::pause() {
     disabled = true;
}
void HttpDaemon::resume() {
    disabled = false;
}


void HttpDaemon::readClient()
{
    if (disabled)
        return;

    // This slot is called when the client sent data to the server. The
    // server looks if it was a get request and sends a very simple HTML
    // document back.
    QTcpSocket* socket = (QTcpSocket*)sender();
    if (socket->canReadLine()) {
        QStringList tokens = QString(socket->readLine()).split(QRegExp("[ \r\n][ \r\n]*"));

        qDebug() << tokens;

        if (tokens[0] == "GET") {
            QTextStream os(socket);
            os.setAutoDetectUnicode(true);

            // Retorna dados de acordo com a rota solicitada processamento de acordo com a rota
            QString rota;
            rota = tokens[1];

            os << this->processRoutes(rota);

//            os << "HTTP/1.0 200 Ok\r\n"
//                "Content-Type: text/html; charset=\"utf-8\"\r\n"
//                "\r\n"
//                "<h1>Nothing to see here</h1>\n"
//                << QDateTime::currentDateTime().toString() << "\n";
            socket->close();

            //QtServiceBase::instance()->logMessage("Wrote to client");

            if (socket->state() == QTcpSocket::UnconnectedState) {
                delete socket;
                //QtServiceBase::instance()->logMessage("Connection closed");
            }
        }
    }
}

void HttpDaemon::discardClient()
{
    QTcpSocket* socket = (QTcpSocket*)sender();
    socket->deleteLater();

    //QtServiceBase::instance()->logMessage("Connection closed");
}

QString HttpDaemon::getInstalaHash()
{
    QString retorno;
    retorno = "HTTP/1.0 200 Ok\r\n"
        "Content-Type: text/plain; charset=\"utf-8\"\r\n"
        "\r\n"
        "CERTO\n";

    return retorno;
}

QString HttpDaemon::getDefaultRoute()
{
    QString retorno;
    retorno = "HTTP/1.0 200 Ok\r\n"
        "Content-Type: text/plain; charset=\"utf-8\"\r\n"
        "\r\n"
        "OK\n";

    retorno += QDateTime::currentDateTime().toString();
    retorno += "\n";

    return retorno;
}

QString HttpDaemon::getErro()
{
    QString retorno;

    retorno = "HTTP/1.0 200 Ok\r\n"
        "Content-Type: text/plain; charset=\"utf-8\"\r\n"
        "\r\n"
        "OK\n";

    retorno += QDateTime::currentDateTime().toString();
    retorno += "\n";

    return retorno;
}

QString HttpDaemon::processRoutes(const QString &rota)
{
    // Processa a rota definida na variável executando um método para cada rota
    qDebug() << rota;
    if (rota == ROUTE_HASH) {
        return this->getInstalaHash();
    } else if (rota == ROUTE_ERRO) {
        return this->getErro();
    } else {
        return this->getDefaultRoute();
    }
}
