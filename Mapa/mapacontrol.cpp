#include "mapacontrol.h"

/**
 * @brief MapaControl::MapaControl
 * @param parent QObject*
 *
 * Inicializa atributos que podem ser usados durante o uso do objeto.
 */
MapaControl::MapaControl(QObject *parent) : QObject(parent)
{
    QString folder = CCacic::getValueFromRegistry("Lightbase", "Cacic", "mainFolder").toString();
    mainFolder = !folder.isEmpty() && !folder.isNull() ? folder : Identificadores::ENDERECO_PATCH_CACIC;
}

/**
 * @brief MapaControl::~MapaControl
 */
MapaControl::~MapaControl()
{
    delete interface;
}

///**
// * @brief MapaControl::args2Map
// * @param args QStringList
// * @param map QMap<QString,QString>&
// * @return bool com true caso tenha argumentos.
// *
// * Mapeia as opções de linha de comando que o Mapa aceita.
// * Opções:
// * -server=server                 seta o servidor onde estarão informações
// *                                para consulta. Se não setada, a informação
// *                                é buscada no getConfig.json.
// */
//bool MapaControl::args2Map(QStringList args, QMap<QString, QString> &map)
//{
//    bool hasArgument = false;

//    foreach(QString aux, args) {
//        QStringList auxList = aux.split("=");
//        if ((auxList.size() > 1) && (auxList.at(0).at(0) == '-'))
//            map[auxList.at(0).mid(1)] = auxList.at(1);
//        else if (aux.at(0) == '-')
//            map["default"] = aux.mid(1);
//    }

//    if ( map.isEmpty() ) {
//        hasArgument = false;
//    } else if( map.contains("server") && !map["server"].isEmpty()) {
//        hasArgument = true;
//    }
//    return hasArgument;
//}

/**
 * @brief MapaControl::getMapa
 * @return bool com true se há permissão para executar o Mapa
 *
 * Realiza a comunicação com o Gerente que permitirá ou não
 * o Mapa ser executado.
 */
bool MapaControl::getMapa()
{
    QJsonObject configJson = CCacic::getJsonFromFile(this->mainFolder + "getConfig.json");
    if(configJson.contains("agentcomputer")) {
        QJsonObject mapa = configJson["agentcomputer"].toObject()["mapa"].toObject();

        if(mapa.contains("col_patr")) {
            return mapa["col_patr"].toBool();
        }
    }

    return false;
}

/**
 * @brief MapaControl::run
 * @param args QStringList
 *
 * Inicia a execução do MapaControl.
 */
void MapaControl::run(QStringList args)
{
//    getMapaP();

qDebug() << "MapaControl::run():" + mainFolder + "getConfig.json";
    QJsonObject getConfigJson = CCacic::getJsonFromFile(mainFolder + "getConfig.json");
    if ( !getConfigJson.isEmpty()
         && getMapa()
         ) {
        interface = new Mapa(getConfigJson["agentcomputer"].toObject()["mapa"].toObject());
        interface->show();
    } else {
        qDebug () << "Falha ao pegar informações do arquivo de configuração.";
        exit(0);
    }
}

bool MapaControl::getMapaP()
{
    bool ok = false;
    CACIC_Computer computer;
    QJsonObject sentJson;
    QJsonObject configs = CCacic::getJsonFromFile(mainFolder + "/getConfig.json");
    CacicComm *OCacicComm = new CacicComm(LOG_DAEMON, mainFolder);

    sentJson["computador"] = computer.toJsonObject();
    sentJson["request"] = QJsonValue::fromVariant(QString("getMapa"));
    if (!sentJson.isEmpty()){
        QJsonObject retornoEnvio;
        QString server = CCacic::getValueFromRegistry("Lightbase", "Cacic", "applicationUrl").toString();
        if( server.isNull() || server.isEmpty() ) {
            server = configs["agentcomputer"].toObject()["applicationUrl"].toString();
            if (!server.isNull() && !server.isEmpty()) {
            } else {
//                logcacic->escrever(LogCacic::ErrorLevel, "Não foi possível recuperar a url para comunicação."
//                                   "Para consertar, é necessário executar o comando a seguir no terminal/cmd: install-cacic -configure -host=nova_url");
//                logcacic->escrever(LogCacic::ErrorLevel, "Problemas ao recuperar url para comunicação.");
                return false;
            }
        }
        OCacicComm->setUrlGerente(server);
        OCacicComm->setUsuario(CCacic::getValueFromRegistry("Lightbase", "Cacic", "usuario").toString());
        OCacicComm->setPassword(CCacic::getValueFromRegistry("Lightbase", "Cacic", "password").toString());

        retornoEnvio = OCacicComm->comm(ROTA_MAPA_GETMAPA, &ok, sentJson , true);
qDebug() << "getMapa: resposta da comunicacao\n\t" << retornoEnvio;
        if( !ok ) {
            return false;
        } else if(retornoEnvio.contains("error") ||
               ( retornoEnvio.contains("reply") && retornoEnvio["reply"].isString())  ) {
            ok = false;
        } else if(retornoEnvio.contains("reply")) {
            QJsonObject reply = retornoEnvio["reply"].toObject();
            if(reply.contains("col_patr")) {
                if(!configs.isEmpty()){
                    QJsonObject agent = configs["agentcomputer"].toObject();
                    QJsonObject mapa;
                    if (reply.contains("ldap")){
                        mapa["ldap"] = reply["ldap"];
                    }
                    mapa["col_patr"] = reply["col_patr"];
                    agent["mapa"] = mapa;
                    configs["agentcomputer"] = agent;
                    CCacic::setJsonToFile(configs,mainFolder + "getConfig.json");
qDebug() << mainFolder + "getConfig.json";
                } else {
//                    logcacic->escrever(LogCacic::ErrorLevel, "Falha ao ler arquivo de configurações.");
                }
                ok = reply["col_patr"].toBool();
            }
        }
    }

    return ok;

}

