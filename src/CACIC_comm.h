#ifndef CACIC_COMM_H
#define CACIC_COMM_H

#include <QString>

class CacicComm {
public:
    QString comm(QString urlGerente, request, localFolderName);

};

QString CacicComm::comm(QString urlGerente, request, localFolderName){
    QString retorno        = '0',
            gerColsInfPath = localFolderName + "/gercols.inf",
            chksisInfPath  = localFolderName + "/chksis.inf";
    //implementar
    return retorno;
}

#endif // CACIC_COMM_H
