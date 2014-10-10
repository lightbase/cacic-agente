#include "checkmodules.h"

CheckModules::CheckModules(const QString &workingPath)
{
    QJsonObject configFile;
    configFile = oCacic.getJsonFromFile(workingPath + "/getConfig.json");
    if (!configFile.isEmpty()) {
        QJsonArray modulos;
        modulos = configFile["agentcomputer"].toObject()["modulos"].toArray();
        foreach (QJsonValue modulo, modulos){
            modules[modulo.toObject()["nome"].toString()] = modulo.toObject()["hash"].toString();
        }
    }
    //TODO: Completar constructor. Agora que tenho os nomes dos módulos e os hashs, fazer a verificação.
    qDebug() << modules;
}

bool CheckModules::start(){
    return true;
}

//QStringList CheckModules::verificaModulos(){
//    // Compara o novo arquivo de configuração com um antigo e se forem diferentes
//    // mantem o mais recente; caso iguais simplesmente apaga o novo.
//    QFile *fileOld;
//    QFile *fileNew;

//    fileOld = new QFile(this->applicationDirPath + "/getConfig.json");
//    fileNew = new QFile(this->applicationDirPath + "/getConfigNew.json");

//    if( fileOld->exists() && fileNew->exists() ){
//        if( Md5IsEqual(QVariant::fromValue(fileOld), QVariant::fromValue(fileNew)) ) {
//            fileNew->remove();
//        } else {
//            // Renomeia getConfigNew.json para getConfig.json
//            fileOld->remove();
//            fileNew->rename("getConfigNew.json","getConfig.json");
//        }
//        jsonConfig = ccacic->getJsonFromFile(this->applicationDirPath + "/getConfig.json");
//    } else if( fileOld->exists() ){
//        jsonConfig = ccacic->getJsonFromFile(this->applicationDirPath + "/getConfig.json");
//    } else {
//        QLogger::QLog_Error("Cacic Daemon (Timer)", "Arquivo de configuração não criado.");
//    }
//    delete fileOld;
//    delete fileNew;
//    lerArquivoConfig(jsonConfig["agentcomputer"].toObject());

//    QStringList nomesModulos;

//    int countExecNotFound = 0;
//    QMap<QString, QString>::const_iterator moduloIterator = moduleMap.constBegin();
//    while (moduloIterator != moduleMap.constEnd()) {
//        QString nomeModulo = moduloIterator.value();
//        QString hashModulo = moduloIterator.key();
//        // Calcula hash do binario atual
//#if defined(Q_OS_WIN)
//        fileOld = new QFile(this->applicationDirPath + "/" + nomeModulo + ".exe");
//#else
//        fileOld = new QFile(this->applicationDirPath + "/" + nomeModulo);
//#endif
//        if(!fileOld->exists()) {
//            QLogger::QLog_Error("Cacic Daemon (Timer)", QString("Módulo ").append(nomeModulo).append(" não encontrado."));
//            countExecNotFound++;

//            if( countExecNotFound == moduleMap.size() ) {
//                QLogger::QLog_Error("Cacic Daemon (Timer)", "Não foi possível encontrar nenhum módulo executável!");
//                return QStringList();
//            }

//            // pula para o próximo módulo no moduloMap
//            moduloIterator++;
//            continue;
//        }

//        QString oldMd5 = QString(QCryptographicHash::hash(fileOld->readAll(),QCryptographicHash::Md5).toHex());
//        if ( oldMd5 != hashModulo ) {

//#if defined(Q_OS_WIN)
//            fileOld->rename(this->applicationDirPath + "/" + nomeModulo + ".exe",
//                            this->applicationDirPath + "/" + nomeModulo + "Old.exe");
//#elif defined(Q_OS_LINUX)
//            fileOld->rename(this->applicationDirPath + "/" + nomeModulo,
//                            this->applicationDirPath + "/" + nomeModulo + "Old");
//#endif

//            // Download nova versão do executável
//            QList<QMap<QString,QString> >::const_iterator metodosIterator = metodosDownload.constBegin();
//            bool downloadSucess = false;
//            while ( !downloadSucess && metodosIterator != metodosDownload.constEnd() ) {
//                if( metodosIterator->value("tipo") == "ftp" || metodosIterator->value("tipo") == "" ) {
//                    if ( OCacicComm->ftpDownload( metodosIterator->value("url"), metodosIterator->value("path") )  )
//                        downloadSucess = true;
//                } else if ( metodosIterator->value("tipo") == "http" ) {
//                    if( OCacicComm->httpDownload( metodosIterator->value("url"), metodosIterator->value("path") )  )
//                        downloadSucess = true;
//                }
//                metodosIterator++;
//            }
//            fileOld->remove();
//            delete fileOld;
//        }

//        nomesModulos.append(nomeModulo);

//        moduloIterator++;
//    }

//    return nomesModulos;
//}

//void CheckModules::lerArquivoConfig ( const QJsonObject& jsonConfig )
//{
//    /* lê json de configurações e armazena quais módulos executáveis.
//     * E faz o mesmo tipo de comparação de hashs, com o fim de:
//     * ou mantem o binário do módulo ou baixa um novo.
//     */
//    foreach( QJsonValue individualModule, jsonConfig["modulos"].toArray() ) {
//        QString moduloKey, moduloValue;

//        moduloKey = individualModule.toObject()["hash"].toString();
//        moduloValue = individualModule.toObject()["nome"].toString();

//        moduleMap.insert(moduloKey, moduloValue);
//    }

//    if ( jsonConfig["metodoDownload"].isArray() ) {

//        foreach (QJsonValue individualMetodo, jsonConfig["metodoDownload"].toArray() ) {
//            QMap<QString, QString> newEntry;

//            newEntry.insert(QString("tipo"), individualMetodo.toObject()["tipo"].toString() );
//            newEntry.insert(QString("url"), individualMetodo.toObject()["url"].toString() );
//            newEntry.insert(QString("path"), individualMetodo.toObject()["path"].toString() );
//            newEntry.insert(QString("usuario"), individualMetodo.toObject()["usario"].toString() );
//            newEntry.insert(QString("senha"), individualMetodo.toObject()["senha"].toString() );

//            metodosDownload.append( newEntry );
//        }
//    } else {
//        QMap<QString, QString> newEntry;

//        newEntry.insert(QString("tipo"), jsonConfig["metodoDownload"].toObject()["tipo"].toString() );
//        newEntry.insert(QString("url"), jsonConfig["metodoDownload"].toObject()["url"].toString() );
//        newEntry.insert(QString("path"), jsonConfig["metodoDownload"].toObject()["path"].toString() );
//        newEntry.insert(QString("usuario"), jsonConfig["metodoDownload"].toObject()["usario"].toString() );
//        newEntry.insert(QString("senha"), jsonConfig["metodoDownload"].toObject()["senha"].toString() );

//        metodosDownload.append( newEntry );
//    }
//}
