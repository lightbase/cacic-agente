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
 * @return
 */
bool MapaControl::args2Map(int argc, char *argv[], QMap<QString, QString> &map)
{
    bool hasArgument;

    QStringList args;
    for (int i = 0; i<argc; i++)
        args.append(argv[i]);

    for (int i = 0; i<argc; i++){
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
        if( ok == false ) {
            return ok;
        } else if(retornoEnvio.contains("error") ||
               ( retornoEnvio.contains("reply") && retornoEnvio["reply"].isString())  ) {
            ok = false;
        } else if(retornoEnvio.contains("reply")) {
            QJsonObject reply = retornoEnvio["reply"].toObject();
            if(reply.contains("objectClass") && reply["objectClass"] == "getMapa" ) {
                ok = reply["col_patrimonio"].toBool();
            }
        }
    }

    return ok;
}

void MapaControl::run(int argc, char *argv[])
{
    QMap<QString, QString> param;

    if ( args2Map(argc, argv, param) ) {
        if ( !param["server"].isEmpty() && !param["server"].isNull() ) { // -server
            if(getMapa(param["server"])){ // Mapa tem permissão do gerente?
                if (!param["ldap"].isEmpty() && !param["ldap"].isNull()) { // -server e -ldap
                    if(param["ldap"] == "true")
                        interface = new Mapa(true);
                    else
                        interface = new Mapa(false);
                    Mapa* mapa = static_cast<Mapa*>(interface);
                    mapa->setComm(param["server"]);
                    interface->show();
                } else { // -server sem -ldap
                    interface = new Mapa(false);
                    Mapa* mapa = static_cast<Mapa*>(interface);
                    mapa->setComm(param["server"]);
                    interface->show();
                }
            } else {
                exit(0);
            }
        } else if (!param["ldap"].isEmpty() && !param["ldap"].isNull()) { // -ldap
            if(param["ldap"] == "true")
                interface = new Mapa(true);
            else
                interface = new Mapa(false);

            QJsonObject getConfigJson = CCacic::getJsonFromFile("getConfig.json");
            if ( !getConfigJson.isEmpty() && getMapa(getConfigJson["applicationUrl"].toString()) ) {
                Mapa* mapa = static_cast<Mapa*>(interface);
                mapa->setComm(getConfigJson["applicationUrl"].toString());
                interface->show();
            } else {
                exit(0);
//                emit finished();
//                return;
            }
        } else if (!param["custom"].isEmpty() && !param["custom"].isNull() ) {
            // TODO
        }
    } else {
        interface = new Mapa();

        QJsonObject getConfigJson = CCacic::getJsonFromFile("getConfig.json");
        if ( !getConfigJson.isEmpty()&& getMapa(getConfigJson["applicationUrl"].toString()) ) {
            Mapa* mapa = static_cast<Mapa*>(interface);
            mapa->setComm(getConfigJson["applicationUrl"].toString());
            interface->show();
        } else {
            exit(0);
        }
    }
//    exit(0);
}

