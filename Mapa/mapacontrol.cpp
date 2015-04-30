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
 * -ldap=server             Define o endereço de servidor do LDAP.
 * -custom                  Consulta o arquivo de configuração criar
 *                          um formulário customizado.
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
    } else if( map.contains("ldap") && !map["ldap"].isEmpty() ||
               map.contains("default") && map["default"] == "custom" ) {
        hasArgument = true;
    }
    return hasArgument;
}

int MapaControl::run(int argc, char *argv[])
{
    QMap<QString, QString> param;

    if ( args2Map(argc, argv, param) ) {
        // TODO: opções com argumento;
        emit finished();
    } else {
        Mapa *mapa = new Mapa();
        mapa->show();
    }

}

