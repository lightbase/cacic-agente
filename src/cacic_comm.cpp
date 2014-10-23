#include "cacic_comm.h"

CacicComm::CacicComm ()
{
}

CacicComm::CacicComm (const QString &urlGerente,          const QString &operatingSystem,     const QString &computerSystem,  const QString &csCipher,
                      const QString &csDebug,             const QString &csCompress,          const QString &httpUserAgent,   const QString &moduleFolderName,
                      const QString &moduleProgramName,   const QString &networkConfiguration,const QString &phpAuthPw,       const QString &phpAuthUser,
                      const QString &so,                  const QString &cacicVersion,        const QString &gercolsVersion)
{
    this->setUrlGerente(urlGerente);
    params.addQueryItem("OperatingSystem", operatingSystem);
    params.addQueryItem("ComputerSystem",computerSystem);
    params.addQueryItem("cs_cipher",csCipher);
    params.addQueryItem("cs_debug",csDebug);
    params.addQueryItem("cs_compress",csCompress);
    params.addQueryItem("HTTP_USER_AGENT",httpUserAgent);
    params.addQueryItem("ModuleFolderName",moduleFolderName);
    params.addQueryItem("ModuleProgramName",moduleProgramName);
    params.addQueryItem("NetworkAdapterConfiguration",networkConfiguration);
    params.addQueryItem("PHP_AUTH_PW",phpAuthPw);
    params.addQueryItem("PHP_AUTH_USER",phpAuthUser);
    params.addQueryItem("te_so",so);
    params.addQueryItem("te_versao_cacic",cacicVersion);
    params.addQueryItem("te_versao_gercols",gercolsVersion);


}

QJsonObject CacicComm::comm(QString route, bool *ok, const QJsonObject &json, bool isSsl)
{
    *ok = false;
    QByteArray data;
    QNetworkRequest req;
    QUrl url;
    QString strReply;
    QJsonObject jsonObj;
    if (isSsl){
        url = urlSsl.isEmpty() ? "https://" + this->urlGerente + route : this->urlSsl + route;
        if (!url.isValid()){
            jsonObj["error"] = QVariant("Invalid Url").toJsonValue();
            return jsonObj;
        }
        req.setSslConfiguration(QSslConfiguration::defaultConfiguration());
    } else
        url = "http://" + this->urlGerente + route;

    req.setUrl(url);
    req.setHeader(QNetworkRequest::LocationHeader, "Cacic Agente");
    if (json.empty()){
        //se não for passado QJson, será mandado os valores do get/test antigo. (Será retirado depois)
        req.setHeader(QNetworkRequest::ContentTypeHeader, "application/x-www-form-urlencoded");
        data.append(params.toString());
    } else {
        // Add JSON
        QJsonDocument d(json);
        data.append(d.toJson());
        req.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");
    }
    QEventLoop eventLoop;
    QNetworkAccessManager mgr;
    QObject::connect(&mgr, SIGNAL(finished(QNetworkReply*)), &eventLoop, SLOT(quit()));

    QNetworkReply *reply = mgr.post(req, data);
    if (!reply->sslConfiguration().isNull()){
        reply->ignoreSslErrors();
    }
    eventLoop.exec(); // sai do looping chamando o "finished()".

    jsonObj.insert("codestatus", QJsonValue::fromVariant(reply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toString()));

    if (reply->error() == QNetworkReply::NoError) {
        //se não houver erro, grava o retorno;
        QVariant replyVariant = reply->readAll();
        QJsonDocument replyDocument = QJsonDocument::fromJson(replyVariant.toByteArray());
        jsonObj["reply"] = (!replyDocument.isNull()) ?
                            replyDocument.object() :
                            QJsonValue::fromVariant(replyVariant.toString());
        *ok = true;
        delete reply;
    } else {
        strReply = reply->errorString();
        jsonObj.insert("error", QJsonValue::fromVariant(strReply));
        delete reply;
    }
    return jsonObj;
}


/* commStatus
 * execulta um teste do tipo GET na urlGerente;
 * @return retorna o resultado da conexão HTTP:
 *      exemplo: 200 OK
 */
bool CacicComm::commStatus(){
    // cria um event-loop temporario
    QEventLoop eventLoop;
    // "quit()" o event-loop, when the network request "finished()"
    QNetworkAccessManager mgr;
    QObject::connect(&mgr, SIGNAL(finished(QNetworkReply*)), &eventLoop, SLOT(quit()));
    // a requisição HTTP
    QUrl url = "http://" + urlGerente;
    QNetworkRequest req( url );
    req.setHeader(QNetworkRequest::LocationHeader, "Cacic Agente");
    QNetworkReply *reply = mgr.get(req);
    eventLoop.exec(); // sai do looping chamando o "finished()".

    QString reason = reply->attribute( QNetworkRequest::HttpReasonPhraseAttribute ).toString();
    if (reply->attribute( QNetworkRequest::HttpStatusCodeAttribute ).isValid()){
//            qDebug() << "Status:" << reply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt() << reason;
        return true;
    }else{
        reason = reply->attribute( QNetworkRequest::HttpReasonPhraseAttribute ).toString();
//              qDebug() << "Error:" << reason;
        return false;
    }
}

/* login
 * realiza login com usuario e senha
 * @return retorna json com sessão e chave de criptografia
 */
QJsonObject CacicComm::login(bool *ok) {
    *ok = false;
    // Cria dados de login
    QVariantMap login;
    login["user"] = this->usuario;
    login["password"] = this->password;
    // Cria conexão e retorna Json da sessão
    QJsonObject retorno = this->comm("/ws/neo/getLogin", ok, QJsonObject::fromVariantMap(login), true);
    if (*ok)
        this->session = retorno["reply"].toObject()["session"].toString();
    return retorno;
}

/* fileDownload( QString path )
 *
 * Faz o download de um arquivo usando o protocolo mode
 * a partir da url do gerente e do caminho até o arquivo.
 */
bool CacicComm::fileDownload(const QString &mode, const QString &path, const QString &pathDownload){
    QEventLoop eventLoop;
    QNetworkAccessManager manager;
    QNetworkRequest request;
    QNetworkReply *reply;

    QObject::connect(&manager, SIGNAL(finished(QNetworkReply*)), this, SLOT(fileDownloadFinished(QNetworkReply*)) );
    QObject::connect(&manager, SIGNAL(finished(QNetworkReply*)), &eventLoop, SLOT(quit()) );

    QStringList splitPath = path.split("/");

    fileHandler = new QFile(pathDownload + splitPath[splitPath.size() - 1]);
    if( !fileHandler->open(QIODevice::WriteOnly) ) {
        qDebug() << "fileDownload: fileHandler nâo pode abrir arquivo.";
        return false;
    }

    QUrl url(urlGerente);
    url.setScheme(mode);
    url.setPath(path);
    if (!this->ftpUser.isEmpty())
        url.setUserName(ftpUser);
    if (!this->ftpPass.isEmpty())
        url.setPassword(ftpPass);

    request.setUrl(url);

    reply = manager.get(request);

    eventLoop.exec();

    delete fileHandler;
    delete reply;

    return true;
}


bool CacicComm::fileDownload(const QString &mode, const QString &urlServer, const QString &path, const QString &pathDownload)
{
    QStringList splitPath = path.split("/");

    fileHandler = new QFile((!pathDownload.isEmpty() ? pathDownload + "/" : "") + splitPath[splitPath.size() - 1]);

    if( !fileHandler->open(QIODevice::WriteOnly) ) {
        qDebug() << "ftpDownload: fileHandler nâo pode abrir arquivo.";
    }

    QString urlParsed = urlServer;
    if ( urlParsed.startsWith("http://") || urlParsed.startsWith("ftp://") ) {
        int indexHost = urlParsed.indexOf(":") + 3;
        urlParsed = urlParsed.mid(indexHost);
    }
    QUrl url(mode + "://" + (urlParsed.endsWith("/") ? urlParsed : urlParsed + "/") + path);
//    url.setScheme(mode);
//    url.setUrl(urlServer);
//    url.setPath(path);
    if (!this->ftpUser.isEmpty())
        url.setUserName(ftpUser);
    if (!this->ftpPass.isEmpty())
        url.setPassword(ftpPass);

    startRequest(url);

    return true;
}

void CacicComm::startRequest(QUrl url)
{

    QEventLoop eventLoop;
    QNetworkAccessManager qnam;

    reply = qnam.get(QNetworkRequest(url));
    connect(reply, SIGNAL(finished()),
            this, SLOT(fileDownloadFinished()));
    connect(reply, SIGNAL(readyRead()),
            this, SLOT(fileDownloadReadyRead()));
    connect(this, SIGNAL(quitLoop()),
            &eventLoop, SLOT(quit()) );

    eventLoop.exec();
}

void CacicComm::fileDownloadFinished()
{
    fileHandler->flush();

    fileHandler->setPermissions(QFileDevice::ReadOwner |
                                QFileDevice::WriteOwner|
                                QFileDevice::ExeOwner  |
                                QFileDevice::ReadUser  |
                                QFileDevice::ExeUser);
    fileHandler->close();

    reply->close();

    delete reply;
    reply = 0;

    delete fileHandler;
    fileHandler = 0;
    emit quitLoop();
}

void CacicComm::fileDownloadReadyRead()
{
    if (fileHandler)
        fileHandler->write(reply->readAll());
}

QString CacicComm::getFtpPass() const
{
    return ftpPass;
}

void CacicComm::setFtpPass(const QString &value)
{
    ftpPass = value;
}

QString CacicComm::getFtpUser() const
{
    return ftpUser;
}

void CacicComm::setFtpUser(const QString &value)
{
    ftpUser = value;
}

bool CacicComm::ftpDownload( const QString &path, const QString &pathDownload ){
    return fileDownload("ftp", path, pathDownload);
}

bool CacicComm::ftpDownload( const QString &urlServer, const QString &path, const QString &pathDownload ){
    return fileDownload("ftp", urlServer, path, pathDownload);
}

bool CacicComm::httpDownload( const QString &path, const QString &pathDownload ){
    return fileDownload("http", path, pathDownload);
}

bool CacicComm::httpDownload( const QString &urlServer, const QString &path,const QString &pathDownload ){
    return fileDownload("http", urlServer, path, pathDownload);
}

QString CacicComm::getUrlSsl (){
    return this->urlSsl;
}

void CacicComm::setUrlSsl(const QString &value){
    this->urlSsl = value;
}

QString CacicComm::getUrlGerente (){
    return this->urlGerente;
}

void CacicComm::setUrlGerente(QString value){
    if (value.contains("://", Qt::CaseInsensitive)){
        value = value.mid(value.indexOf("://") + 3);
    }
    if (value.endsWith("/")){
        value.remove(value.size()-1, 1);
    }
    this->urlGerente = value;
}

QString CacicComm::getPassword()
{
    return this->password;
}

void CacicComm::setPassword(const QString &value)
{
    this->password = value;
}
QString CacicComm::getUsuario()
{
    return this->usuario;
}

void CacicComm::setUsuario(const QString &value)
{
    this->usuario = value;
}


