#include "mapacontrol.h"

MapaControl::MapaControl(QObject *parent) : QObject(parent)
{
    QString folder = CCacic::getValueFromRegistry("Lightbase", "Cacic", "mainFolder").toString();
    mainFolder = !folder.isEmpty() && !folder.isNull() ? folder : Identificadores::ENDERECO_PATCH_CACIC;
}

MapaControl::~MapaControl()
{
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
bool MapaControl::getMapa()
{
    QJsonObject retornoEnvio = CCacic::getJsonFromFile(this->mainFolder + "getConfig.json");

    if(retornoEnvio.contains("agentcomputer")) {
        QJsonObject mapa = retornoEnvio["agentcomputer"].toObject()["mapa"].toObject();

        if(mapa.contains("col_patr")) {
            return mapa["col_patr"].toBool();
        }
    }

    return false;
}

void MapaControl::run(QStringList args)
{
    QMap<QString, QString> param;

    if ( args2Map(args, param) ) {
        if ( !param["server"].isEmpty() && !param["server"].isNull() ) { // -server
            if(getMapa()){ // Mapa tem permissão do gerente?
                if (!param["ldap"].isEmpty() && !param["ldap"].isNull()) { // -server e -ldap
                    if(param["ldap"] == "true")
                        interface = new Mapa();
                    else
                        interface = new Mapa();
                    interface->show();
                } else { // -server sem -ldap
                    interface = new Mapa();
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
            if ( !getConfigJson.isEmpty() && getMapa() ) {
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
             && getMapa()
             ) {
            interface = new Mapa();
            interface->show();
        } else {
            qDebug () << "Falha ao pegar informações do arquivo de configuração.";
            exit(0);
        }
    }
}

