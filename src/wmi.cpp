#include "wmi.h"
#ifdef Q_OS_WIN

wmi::wmi()
{
}

/***********************************************************************
 * Faz a pesquisa WMI de acordo com a classe e os parametros repassados.
 ***********************************************************************/
QJsonValue wmi::wmiSearch(QString classe, QStringList params)
{
    QJsonValue wmiReturn;
    QJsonObject itemWmi;
    QJsonArray arrayItens;
    QString paramsString;
    if (!params.empty()){
        for(int i = 0; i!=params.size();i++)
            paramsString.append(params.at(i) + ((i != params.size() - 1) ? "," : ""));
    }
    QString query = "SELECT " + ((!paramsString.isEmpty()) ? paramsString : "*") + " FROM " + classe;
    //conecta ao script (Como já estamos usando QObject, o WbemScripting... não funciona chamando normal,
    // por isso deve ser usado o CoInitializeEx)
    ::CoInitializeEx(NULL, COINIT_MULTITHREADED);
    QAxObject *objIWbemLocator = new QAxObject();
    objIWbemLocator->setControl("WbemScripting.SWbemLocator");
    if (objIWbemLocator->isNull())
        return QJsonValue();
    //conecta ao wmi
    QAxObject *objWMIService = objIWbemLocator->querySubObject("ConnectServer(QString&,QString&)",
                                                                    QString("."),
                                                                    QString("root\\CIMV2")
                                                               );

    //Faz a pesquisa no wmi
    QAxObject* returnList = objWMIService->querySubObject("ExecQuery(QString&)", query);

    QAxObject *enum1 = returnList->querySubObject("_NewEnum");

    IEnumVARIANT* enumInterface = 0;

    enum1->queryInterface(IID_IEnumVARIANT, (void**)&enumInterface);

    enumInterface->Reset();
    int countReturnListObj = returnList->dynamicCall("Count").toInt();
    for (int i = 0; i < countReturnListObj; i++) {
        VARIANT *theItem = (VARIANT*)malloc(sizeof(VARIANT));
        if (enumInterface->Next(1,theItem,NULL) != S_FALSE){
            QAxObject *item = new QAxObject((IUnknown *)theItem->punkVal);
            if(item){
                //pega o retorno da query
                QString instance = item->dynamicCall("GetObjectText_(int)", QVariant(0)).toString();
                QStringList instanceList = instance.split("\n\t");
                //Pra cada linha grava no json os valores com cada tag
                foreach(QString value, instanceList){
                    QStringList valueList = value.split("=");
                    if (valueList.size() > 1){
                        QString tag = valueList.at(0).trimmed();
                        QString aux = valueList.at(1).trimmed();
                        aux.remove("\"");
                        aux.remove(";");
                        aux.remove("\n");
                        //verifica se é lista
                        if(aux.startsWith("{") && aux.endsWith("}")){
                            QStringList auxList = aux.split(",");
                            QJsonArray jList;
                            foreach(QString valueList, auxList){
                                if (valueList.contains("{"))
                                    valueList.remove("{");
                                if (valueList.contains("}"))
                                    valueList.remove("}");
                                if (!valueList.trimmed().isEmpty())
                                jList.append(valueList.trimmed());
                            }
                            itemWmi[tag] = jList;
                        } else {
                            //O último valor sempre volta com "}" no final.
                            if (aux.contains("}"))
                                aux.remove("}");
                            if (!aux.trimmed().isEmpty())
                                itemWmi[tag] = QJsonValue::fromVariant(aux.trimmed());
                        }
                    }
                } // foreach(QString value, instanceList)
            } // if(item)
        } // if (enumInterface->Next(1,theItem,NULL) != S_FALSE)
        //Se houver mais de um objeto, grava em uma lista
        if (countReturnListObj > 1){
            arrayItens.append(QJsonValue(itemWmi));
        }
    }
    objIWbemLocator->clear();
    ::CoUninitialize();
    //se for mais de um objeto, retorna array.
    if (!arrayItens.isEmpty())
        wmiReturn = QJsonValue(arrayItens);
    //senão, se o objeto não for vazio (se tiver coletado algo)
    else if (!itemWmi.isEmpty())
        wmiReturn = QJsonValue(itemWmi);

    return wmiReturn;
}
#endif //def Q_OS_WIN
