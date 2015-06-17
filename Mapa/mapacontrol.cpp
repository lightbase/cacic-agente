#include "mapacontrol.h"

MapaControl::MapaControl(QObject *parent) : QObject(parent)
{

}

MapaControl::~MapaControl()
{

}

/**
 * @brief MapaControl::args2Map
 * @param argc
 * @param argv
 * @param map
 *
 * Esta função mapeia as opções de linha de comando que o Mapa aceita.
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

int MapaControl::run(int argc, char *argv[])
{
    QMap<QString, QString> param;

    if ( args2Map(argc, argv, param) ) {
        if ( !param["server"].isEmpty() && !param["server"].isNull() ) { // -server

            if (!param["ldap"].isEmpty() && !param["ldap"].isNull()) { // -server e -ldap
                if(param["ldap"] == "true")
                    interface = new Mapa(true);
                else
                    interface = new Mapa(false);
                Mapa* mapa = static_cast<Mapa*>(interface);
                mapa->setComm(param["server"]);
                interface->show();
            } else {
                interface = new Mapa(false);
                Mapa* mapa = static_cast<Mapa*>(interface);
                mapa->setComm(param["server"]);
                interface->show();
            }

        } else if (!param["ldap"].isEmpty() && !param["ldap"].isNull()) { // -ldap
            if(param["ldap"] == "true")
                interface = new Mapa(true);
            else
                interface = new Mapa(false);

            QJsonObject getConfigJson = CCacic::getJsonFromFile("getConfig.json");
            if ( !getConfigJson.isEmpty() ) {
                Mapa* mapa = static_cast<Mapa*>(interface);
                mapa->setComm(getConfigJson["applicationUrl"].toString());
                interface->show();
            } else {
                return 0;
            }
        } else if (!param["custom"].isEmpty() && !param["custom"].isNull() ) {
            // TODO
        }
    } else {
        interface = new Mapa();

        QJsonObject getConfigJson = CCacic::getJsonFromFile("getConfig.json");
        if ( !getConfigJson.isEmpty() ) {
            Mapa* mapa = static_cast<Mapa*>(interface);
            mapa->setComm(getConfigJson["applicationUrl"].toString());
            interface->show();
        } else {
            return 0;
        }
    }
}

