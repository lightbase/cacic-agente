#include "ccacic.h"

CCacic::CCacic(QObject *parent) :
    QObject(parent)
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
    QDir folder(path);
    if (folder.exists())
        return folder.remove(path);
    else
        return true;
}
/*jsonValueFromJsonString
 * Pega valor da chave desejada em um json em string;
 * @parameter QString json: json onde será feita a busca;
 * @parameter QString key: chave onde será pego o json;
 * @return QJsonValue: valor solicitado;
 */
QJsonValue CCacic::jsonValueFromJsonString(QString json, QString key)
{
    return QJsonDocument::fromJson(json.toUtf8()).object()[key];
}

bool CCacic::setJsonToFile(QJsonObject json, QString filepath)
{
    QFile configFile(filepath);
    if (!configFile.open(QIODevice::WriteOnly)){
        qDebug() << "Não foi possivel abrir o arquivo.";
    }
    QJsonDocument docJson(json);
    return (configFile.write(docJson.toJson()) != -1);
}

QJsonObject CCacic::getJsonFromFile(QString filepath)
{
    QFile configFile(filepath);
    QJsonObject json;
    if (!configFile.open(QIODevice::ReadOnly))
        qDebug() << "Nao foi possivel ler o arquivo";
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
std::string CCacic::enCrypt(std::string str_in, std::string key, std::string iv) {
    std::string str_out;
    CryptoPP::CFB_Mode<CryptoPP::AES>::Encryption encryption((byte*)key.c_str(), key.length(), (byte*)iv.c_str());
    CryptoPP::StringSource encryptor(str_in, true,
                                     new CryptoPP::StreamTransformationFilter(encryption,
                                        new CryptoPP::Base64Encoder(new CryptoPP::StringSink(str_out),
                                            false // do not append a newline
                                            )
                                        )
                                    );
    //qDebug(QString::fromStdString(str_out).toLocal8Bit());
    return str_out;
}

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
std::string CCacic::deCrypt(std::string str_in, std::string key, std::string iv) {
    std::string str_out;
    CryptoPP::CFB_Mode<CryptoPP::AES>::Decryption decryption((byte*)key.c_str(), key.length(), (byte*)iv.c_str());

    CryptoPP::StringSource decryptor(str_in, true,
                                    new CryptoPP::Base64Decoder(
                                        new CryptoPP::StreamTransformationFilter(decryption,
                                            new CryptoPP::StringSink(str_out))
                                       )
                                     );
    return str_out;
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


/*Getters/Setters
 * End.
 */
