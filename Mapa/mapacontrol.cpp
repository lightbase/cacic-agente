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
 * -ldap=server                   Habilita consulta ao LDAP e seta qual
 *                                o servidor onde estarão informações
 *                                para consulta.
 * -custom=server                 Habilita opção de formulário customizado
 *                                e seta o servidor onde estarão descritos
 *                                os campos.
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
    } else if( (map.contains("ldap") && !map["ldap"].isEmpty()) ||
               (map.contains("custom") && !map["custom"].isEmpty()) ) {
        hasArgument = true;

    }
    return hasArgument;
}

int MapaControl::run(int argc, char *argv[])
{
    QMap<QString, QString> param;

    if ( args2Map(argc, argv, param) ) {
        if ( !param["ldap"].isEmpty() && !param["ldap"].isNull() &&
             !param["custom"].isEmpty() && !param["custom"].isNull() ) {
            // TODO
        } else if (!param["ldap"].isEmpty() && !param["ldap"].isNull()) {
            interface = new Mapa(param["ldap"]);
            interface->show();
        } else if (!param["custom"].isEmpty() && !param["custom"].isNull() ) {
            // TODO
        }
    } else {
        interface = new Mapa();
        interface->show();
    }

}

