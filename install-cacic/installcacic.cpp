#include "installcacic.h"

InstallCacic::InstallCacic(QObject *parent) :
    QObject(parent)
{

}

void InstallCacic::run(QStringList argv, int argc) {
    QMap<QString, QString> args;
    bool ok;
    //valida os parametros repassados
    validaParametros(argv, argc, &ok);
    //se tiver usuario, senha e url
    if (ok){
        oCacicComm.setUrlGerente(this->argumentos["host"]);
        oCacicComm.setUsuario(this->argumentos["user"]);
        oCacicComm.setPassword(this->argumentos["password"]);
        QJsonObject jsonLogin = oCacicComm.login(&ok);
        if (ok){
            //TODO: Conectado. armazenar chave em arquivo e sessao na comm;
            qDebug() << "Logado";
        }
        else
            qDebug() << "falha no login";
    } else {
        std::cout << "Parametros incorretos. ([obrigatorios] <opcional>)\n";
        std::cout << "  [-host=url_gerente]       <Descricao aqui>\n";
        std::cout << "  [-user=usuario]           <Descricao aqui>\n";
        std::cout << "  [-password=senha]         <Descricao aqui>\n";
        std::cout << "  <-help>                   <Descricao aqui>\n";
    }

    emit finished();
}

QMap<QString, QString> InstallCacic::validaParametros(QStringList argv, int argc, bool *ok)
{
    QMap<QString, QString> map;
    for (int i = 0; i<argc; i++){
        QString aux = argv[i];
        QStringList auxList = aux.split("=");
        if ((auxList.at(0).at(0) == '-') && (auxList.size() > 1))
            map[auxList.at(0).mid(1)] = auxList.at(1);
    }
    *ok = (bool) map.contains("host") && map.contains("user") && map.contains("password");
    if (*ok){
        this->argumentos = map;
    }
    return map;
}

QMap<QString, QString> InstallCacic::getArgumentos()
{
    return argumentos;
}

void InstallCacic::setArgumentos(QMap<QString, QString> value)
{
    this->argumentos = value;
}


