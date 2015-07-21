#include "mapacontrol.h"

MapaControl::MapaControl(QObject *parent) : QObject(parent)
{
    QString folder = CCacic::getValueFromRegistry("Lightbase", "Cacic", "mainFolder").toString();
    mainFolder = !folder.isEmpty() && !folder.isNull() ? folder : Identificadores::ENDERECO_PATCH_CACIC;

    oCacicComm = new CacicComm(LOG_MAPA, this->mainFolder);
}

MapaControl::~MapaControl()
{
    delete oCacicComm;
    delete interface;
}

/**
 * @brief MapaControl::args2Map
 * @param argc
 * @param argv
 * @param map
 *
 * Este método mapeia as opções de linha de comando que o Mapa aceita.
 * Opções:
 * -server=server                 seta o servidor onde estarão informações
 *                                para consulta. Se não setada, a informação
 *                                é buscada no getConfig.json.
 * -ldap=true/false               Habilita ou não consulta ao LDAP.
 * @return true caso tenha argumentos.
 */
bool MapaControl::args2Map(QStringList args, QMap<QString, QString> &map)
{
    bool hasArgument = false;

    for (int i = 0; i<args.size(); i++){
        QString aux = args[i];
        QStringList auxList = aux.split("=");
        if ((auxList.at(0).at(0) == '-') && (auxList.size() > 1))
            map[auxList.at(0).mid(1)] = auxList.at(1);
        else if (aux.at(0)== '-')
            map["default"] = aux.mid(1);
    }

    if ( map.isEmpty() ) {
        hasArgument = false;
    } else if( (map.contains("server") && !map["server"].isEmpty()) ||
               (map.contains("server") && !map["ldap"].isEmpty())) {
        hasArgument = true;
    }
    return hasArgument;
}

/**
 * @brief MapaControl::getMapa
 * @param server
 *
 * Este método realiza a comunicação com o Gerente que permitirá ou não
 * o Mapa ser executado.
 *
 * @return
 */
bool MapaControl::getMapa(const QString &server)
{
    bool ok = false;
    CACIC_Computer computer;
    QJsonObject sentJson;

    sentJson["computador"] = computer.toJsonObject();
    sentJson["request"] = QJsonValue::fromVariant(QString("getMapa"));
    if (!sentJson.isEmpty()){
        QJsonObject retornoEnvio;

        oCacicComm->setUrlGerente(server);
        if( !CCacic::getValueFromRegistry("Lightbase", "Cacic", "applicationUrl").isNull() ) {
            oCacicComm->setUsuario(CCacic::getValueFromRegistry("Lightbase", "Cacic", "usuario").toString());
            oCacicComm->setPassword(CCacic::getValueFromRegistry("Lightbase", "Cacic", "password").toString());
        }

        retornoEnvio = oCacicComm->comm(ROTA_MAPA_GETMAPA, &ok, sentJson , false);
qDebug() << "getMapa: resposta da comunicacao\n\t" << retornoEnvio;
        if( !ok ) {
            return false;
        } else if(retornoEnvio.contains("error") ||
               ( retornoEnvio.contains("reply") && retornoEnvio["reply"].isString())  ) {
            ok = false;
        } else if(retornoEnvio.contains("reply")) {
            QJsonObject reply = retornoEnvio["reply"].toObject();
            if(reply.contains("col_patr")) {
                if (reply.contains("ldap")){
                    QJsonObject configs = CCacic::getJsonFromFile(this->mainFolder + "getConfig.json");
                    if(!configs.isEmpty()){
                        QJsonObject agent = configs["agentcomputer"].toObject();
                        agent["ldap"] = reply["ldap"];
                        configs["agentcomputer"] = agent;
                        CCacic::setJsonToFile(configs,this->mainFolder + "getConfig,json");
                    }
                }
                ok = reply["col_patr"].toBool();
            }
        }
    }

    return ok;
}

void MapaControl::run(QStringList args)
{
    QMap<QString, QString> param;

    if ( args2Map(args, param) ) {
        if ( !param["server"].isEmpty() && !param["server"].isNull() ) { // -server
            if(getMapa(param["server"])){ // Mapa tem permissão do gerente?
                if (!param["ldap"].isEmpty() && !param["ldap"].isNull()) { // -server e -ldap
                    if(param["ldap"] == "true")
                        interface = new Mapa();
                    else
                        interface = new Mapa();
                    Mapa* mapa = static_cast<Mapa*>(interface);
                    mapa->setComm(param["server"]);
                    interface->show();
                } else { // -server sem -ldap
                    interface = new Mapa();
                    Mapa* mapa = static_cast<Mapa*>(interface);
                    mapa->setComm(param["server"]);
                    interface->show();
                }
            } else {
                qDebug () << "n tem permissão";
                exit(0);
            }
        } else if (!param["ldap"].isEmpty() && !param["ldap"].isNull()) { // -ldap
            if(param["ldap"] == "true")
                interface = new Mapa();
            else
                interface = new Mapa();

            QJsonObject getConfigJson = CCacic::getJsonFromFile("getConfig.json");
            if ( !getConfigJson.isEmpty() && getMapa(getConfigJson["applicationUrl"].toString()) ) {
                Mapa* mapa = static_cast<Mapa*>(interface);
                mapa->setComm(getConfigJson["applicationUrl"].toString());
                interface->show();
            } else {
                exit(0);
            }
        } else if (!param["custom"].isEmpty() && !param["custom"].isNull() ) {
            // TODO
            qDebug () << "Parâmetros incorretos.";
            exit(0);
        }
    } else {
        QJsonObject getConfigJson = CCacic::getJsonFromFile(mainFolder + "getConfig.json");
        if ( !getConfigJson.isEmpty()
//             && getMapa(getConfigJson["applicationUrl"].toString())
             ) {
            interface = new Mapa();
            Mapa* mapa = static_cast<Mapa*>(interface);
            mapa->setComm(getConfigJson["applicationUrl"].toString());
            interface->show();
        } else {
            qDebug () << "Falha ao pegar informações do arquivo de configuração.";
            exit(0);
        }
    }
}

