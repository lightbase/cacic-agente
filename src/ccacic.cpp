#include "ccacic.h"

CCacic::CCacic()
{
}
/* getValueFromFile
 * Pega valor específico dentro de um arquivo
 * @parameter QString sectionName: nome da seção onde estará a key)
 * @parameter QString keyName: nome da key que está o valor)
 * @parameter QString filePath: caminho do arquivo que será aberto;
 *
 * @return QString: "" (vazio) Caso não encontre,
 *                  "0" caso não seja possível abrir o arquivo;
 */
QString CCacic::getValueFromFile(QString sectionName, QString keyName, QString filePath)
{
    QFile file(filePath);
    QString line = "";
    sectionName = "[" + sectionName + "]";
    keyName     = keyName + "=";
    int sizeKeyName = keyName.size();

    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
        return "0";

    while (!file.atEnd()){
        line = file.readLine();
       if (line.contains(sectionName, Qt::CaseInsensitive)) {
           do {
               line = file.readLine();
               if (line.contains(keyName, Qt::CaseInsensitive)) {
                   file.close();
                   return line.mid(sizeKeyName).trimmed();
               }
           } while (!file.atEnd());
       }
    }
    file.close();
    return "";
}
/*getValueFromTags
 * @parameter QString sectionName: Grupo onde está a chave;
 * @parameter QString keyName: chave que queremos para colocar o valor;
 * @parameter QString value: valor para colocar dentro da chave;
 * @parameter QString filePath: Local do arquivo;
 */
void CCacic::setValueToFile(QString sectionName, QString keyName, QString value, QString filePath)
{
    QSettings settings(filePath, QSettings::IniFormat);
    settings.beginGroup(sectionName);
    settings.setValue(keyName, value);
    settings.endGroup();
}

/*getValueFromTags
 * @parameter QString fullString: string onde será pesquisado o valor desejado;
 * @parameter QString tag: nome da tag onde estará o valor;
 * @parameter QString tagType: padrão "[]", tipo da tag;
 *
 * @return: QString: valor entre as tags.
 */
QString CCacic::getValueFromTags(QString fullString, QString tag, QString tagType) {
    QString tagFim = tagType.mid(0,1) + "/" + tag + tagType.mid(1);
    int tagSize;
    tag = tagType.mid(0,1) + tag + tagType.mid(1);
    tagSize = tag.size();
    return fullString.mid(fullString.indexOf(tag) + tagSize,
                          fullString.indexOf(tagFim) -
                          (fullString.indexOf(tag) + tagSize)).trimmed();
}
/*createFolder
 * @parameter QString path: caminho onde será criado o diretório, sendo criado toda a árvore se necessário.
 * @return bool: true se conseguir ou já existir, false se não.
 */
bool CCacic::createFolder(QString path)
{
    QDir folder(path);
    return folder.mkpath(path);
}
/*deleteFolder
 * @parameter QString path: caminho do diretório;
 * @return bool: true se conseguir ou já não existir, false se não conseguir.
 */
bool CCacic::deleteFolder(QString path)
{
    QDir folder(path);
    if (folder.exists())
        return folder.removeRecursively();
    else
        return true;
}
/*deleteFile
 * @parameter QString path: caminho do arquivo;
 * @return bool: true se conseguir ou já não existir, false se não conseguir.
 */
bool CCacic::deleteFile(QString path)
{
    QFile file(path);
    if (file.exists())
        return file.remove(path);
    else
        return true;
}
/*jsonValueFromJsonString
 * Pega valor da chave desejada em um json em string;
 * @parameter QString json: json onde será feita a busca;
 * @parameter QString key: chave onde será pego o json;
 * @return QJsonValue: valor solicitado;
 */
//QJsonValue CCacic::jsonValueFromJsonString(QString json, QString key)
//{
//    return QJsonDocument::fromJson(json.toUtf8()).object()[key];
//}
/*setJsonToFile
 * Grava json em um arquivo.
 * IMPORTANTE: não parei pra olhar a fundo, mas a princípio é necessário ler o arquivo
 * para pegar o json das informações anteriores, senão informações serão duplicadas ou
 * excluidas;
 * @parameter QJsonObject json: json que será gravado
 * @parameter QString filepath: caminho do arquivo onde será gravado
 *
 */
bool CCacic::setJsonToFile(QJsonObject json, QString filepath)
{
    QFile configFile(filepath);
    if (!configFile.open(QIODevice::WriteOnly)){
        return false;
    }
    QJsonDocument docJson(json);

    return (configFile.write(docJson.toJson()) != -1);
}

/*getJsonFromFile
 * Carrega json de um arquivo.
 * @return QJsonObject: json que será recuperado
 * @parameter QString filepath: caminho do arquivo onde será recuperado
 */
QJsonObject CCacic::getJsonFromFile(QString filepath)
{
    QFile configFile(filepath);
    QJsonObject json;
    if (!configFile.open(QIODevice::ReadOnly)){
        return json;
    }
    json = QJsonDocument::fromJson(configFile.readAll()).object();
    return json;
}

/*enCrypt
 * @parameter std::string str_in: string que será encriptada (url).
 *            std::string key: chave utilizada na encriptação (32 caracteres) 32*8 = 256 bits
 *              *exemplo: qwertyuiopasdfghjklzxcvbnmqwerty
 *            std::string iv: IV (Vetor de Inicialização) deve ser aleatório.
 *              (http://pt.wikipedia.org/wiki/Modo_de_opera%C3%A7%C3%A3o_%28criptografia%29#Vetor_de_inicializa.C3.A7.C3.A3o_.28IV.29)
 *              exemplo de iv: 0123456789123456
 * @return std:string: retorna a string encriptada convertida em base64.
 * */
//QString CCacic::enCrypt(std::string str_in, std::string iv) {
//    std::string str_out;
//    if ((!this->getChaveCrypt().isNull())){
//        std::string key = (!this->getChaveCrypt().isNull()) ? this->getChaveCrypt().toStdString() : "";
//        CryptoPP::CFB_Mode<CryptoPP::AES>::Encryption encryption((byte*)key.c_str(), key.length(), (byte*)iv.c_str());
//        CryptoPP::StringSource encryptor(str_in, true,
//                                         new CryptoPP::StreamTransformationFilter(encryption,
//                                            new CryptoPP::Base64Encoder(new CryptoPP::StringSink(str_out),
//                                                false // do not append a newline
//                                                )
//                                            )
//                                        );
//    }

//    return QString::fromStdString(str_out);
//}

/*deCrypt
// * @parameter std::string str_in: string encriptada convertida em base64.
// *            std::string key: chave utilizada na encriptação (32 caracteres) 32*8 = 256 bits
// *              *exemplo: qwertyuiopasdfghjklzxcvbnmqwerty
// *            std::string iv: IV (Vetor de Inicialização) deve ser aleatório.
// *              *Um IV jamais deve ser utilizado mais de uma vez com a mesma chave.
// *              *(http://pt.wikipedia.org/wiki/Modo_de_opera%C3%A7%C3%A3o_%28criptografia%29#Vetor_de_inicializa.C3.A7.C3.A3o_.28IV.29)
// *              *exemplo de iv: 0123456789123456
// * @return QString: retorna a string desencriptada convertida em base64.
// * */
//QString CCacic::deCrypt(std::string str_in, std::string iv) {
//    std::string str_out;
//    if ((!this->getChaveCrypt().isNull())){
//        std::string key = this->getChaveCrypt().toStdString();
//        CryptoPP::CFB_Mode<CryptoPP::AES>::Decryption decryption((byte*)key.c_str(), key.length(), (byte*)iv.c_str());

//        CryptoPP::StringSource decryptor(str_in, true,
//                                        new CryptoPP::Base64Decoder(
//                                            new CryptoPP::StreamTransformationFilter(decryption,
//                                                new CryptoPP::StringSink(str_out))
//                                           )
//                                         );
//    }
//    return QString::fromStdString(str_out);
//}

/********************************
 * Compara o MD5 de dois arquivos
 ********************************/
bool CCacic::Md5IsEqual(QVariant document01,QVariant document02){
    QString file1 = QString(QCryptographicHash::hash(
                                       (document01.toByteArray()),QCryptographicHash::Md5).toHex());
    QString file2 = QString(QCryptographicHash::hash(
                                        (document02.toByteArray()),QCryptographicHash::Md5).toHex());
    return file1 == file2;
}
/***********************************************************
 * Compara o MD5 de um arquivo com uma string (o MD5 em si)
 ***********************************************************/
bool CCacic::Md5IsEqual(QByteArray document01,QString document02){
    QString file1 = QString(QCryptographicHash::hash(
                                       document01,QCryptographicHash::Md5).toHex());
    QString file2 = document02;

    return file1 == file2;
}

/******************************************
 * Método com funcionamento duvidoso...
 * TO DO: Ou exclui ou deixá-lo funcional.
 ******************************************/
QString CCacic::startProcess(QString pathprogram, bool wait, bool *ok, QStringList arguments)
{
    QProcess process;
    arguments.empty() ? process.start(pathprogram) : process.start(pathprogram, arguments);
    *ok = wait ? process.waitForFinished() : process.waitForStarted();
    return process.errorString();
}

/***********************************************************
 * Salva valores em um registro local do sistema operacional.
 * Mais detalhes verifique QSettings
 ***********************************************************/
void CCacic::setValueToRegistry(QString organization, QString application, QVariantMap values)
{
    QSettings registry(QSettings::SystemScope, organization, application);
    for (QVariantMap::const_iterator i = values.constBegin(); i != values.constEnd(); i++)
        registry.setValue(i.key(), i.value());
    registry.sync();
}

/***********************************************************
 * Recupera valores em um registro local do sistema operacional.
 * Mais detalhes verifique QSettings
 ***********************************************************/
QVariant CCacic::getValueFromRegistry(QString organization, QString application, QString key)
{
    QSettings registry(QSettings::SystemScope, organization, application);
    return registry.value(key);
}

/***********************************************************
 * Remove valores de um registro local do sistema operacional.
 * Mais detalhes verifique QSettings
 ***********************************************************/
void CCacic::removeRegistry(QString organization, QString application)
{
    QSettings registry(QSettings::SystemScope, organization, application);
    registry.clear();
    registry.sync();
}

/*Getters/Setters
 * Begin:
 */
QString CCacic::getCacicMainFolder() const
{
    return cacicMainFolder;
}

void CCacic::setCacicMainFolder(const QString &value)
{
    cacicMainFolder = value;
}
QString CCacic::getMainModuleName() const
{
    return mainModuleName;
}

void CCacic::setMainModuleName(const QString &value)
{
    mainModuleName = value;
}
QString CCacic::getUrlGerente() const
{
    return urlGerente;
}

void CCacic::setUrlGerente(const QString &value)
{
    urlGerente = value;
}
QString CCacic::getGerColsInfFilePath() const
{
    return gerColsInfFilePath;
}

void CCacic::setGerColsInfFilePath(const QString &value)
{
    gerColsInfFilePath = value;
}
QString CCacic::getChksisInfFilePath() const
{
    return chksisInfFilePath;
}

void CCacic::setChksisInfFilePath(const QString &value)
{
    chksisInfFilePath = value;
}
QString CCacic::getChaveCrypt() const
{
    return chaveCrypt;
}

void CCacic::setChaveCrypt(const QString &value)
{
    chaveCrypt = value;
}

QString CCacic::convertDouble(const double &number, const int &precision)
{
    std::ostringstream ss;
    ss.precision(precision);
    ss << std::fixed << number;

    return QString::fromStdString(ss.str()).trimmed();
}

std::string CCacic::genRandomString(const int &len)
{
    char* s;

    s = (char *) malloc(sizeof(char) * len);

    static const char alphanum[] =
        "0123456789"
        "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
        "abcdefghijklmnopqrstuvwxyz";

    for (int i = 0; i < len; ++i) {
        s[i] = alphanum[rand() % (sizeof(alphanum) - 1)];
    }

    s[len] = 0;
    return std::string(s);
}

/*Getters/Setters
 * End.
 */
