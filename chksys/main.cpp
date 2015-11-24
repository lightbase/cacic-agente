#include "chksys.h"
#include "servicecontroller.h"

int main(int argc, char **argv)
{
    chksys service(argc, argv);
    service.setServiceDescription(CHKSYS_NAME);
    service.setStartupType(QtServiceController::AutoStartup);
    service.setServiceFlags(chksys::Default);

#ifdef Q_OS_WIN
    ServiceController *serviceHandler = new ServiceController(QString("CheckCacic").toStdWString());
    if (!serviceHandler->isInstalled()){
        serviceHandler->install(QString(Identificadores::ENDERECO_PATCH_CACIC + "chksys.exe").toStdWString(),
                                QString("CheckCacic").toStdWString());
        QThread::sleep(3);
    }
    delete serviceHandler;
#endif
    return service.exec();
}
