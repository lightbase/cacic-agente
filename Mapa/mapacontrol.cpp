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

/**
 * @brief MapaControl::args2Map
 * @param args QStringList
 * @param map QMap<QString,QString>&
 * @return bool com true caso tenha argumentos.
 *
 * Mapeia as opções de linha de comando que o Mapa aceita.
 * Opções:
 * -server=server                 seta o servidor onde estarão informações
 *                                para consulta. Se não setada, a informação
 *                                é buscada no getConfig.json.
 */
bool MapaControl::args2Map(QStringList args, QMap<QString, QString> &map)
{
    bool hasArgument = false;

    foreach(QString aux, args) {
        QStringList auxList = aux.split("=");
        if ((auxList.size() > 1) && (auxList.at(0).at(0) == '-'))
            map[auxList.at(0).mid(1)] = auxList.at(1);
        else if (aux.at(0) == '-')
            map["default"] = aux.mid(1);
    }

    if ( map.isEmpty() ) {
        hasArgument = false;
    } else if( map.contains("server") && !map["server"].isEmpty()) {
        hasArgument = true;
    }
    return hasArgument;
}

/**
 * @brief MapaControl::getMapa
 * @return bool com true se há permissão para executar o Mapa
 *
 * Realiza a comunicação com o Gerente que permitirá ou não
 * o Mapa ser executado.
 */
bool MapaControl::getMapa()
{
    QJsonObject configJson = CCacic::getJsonFromFile(this->mainFolder + "/getConfig.json");

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
    QMap<QString, QString> param;

    if ( args2Map(args, param) ) {
        if ( !param["server"].isEmpty() && !param["server"].isNull() ) { // -server
            if(getMapa()){ // Mapa tem permissão do gerente?
                interface = new Mapa();
                interface->show();
            } else {
                qDebug () << "n tem permissão";
                exit(0);
            }
        } else {
            qDebug () << "Parâmetros incorretos.";
            exit(0);
        }
    } else {
        QJsonObject getConfigJson = CCacic::getJsonFromFile(mainFolder + "/getConfig.json");
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

