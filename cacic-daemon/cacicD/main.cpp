#include <QCoreApplication>
#include "cacicd.h"

int main(int argc, char **argv)
{
    cacicD service(argc, argv);
    service.setServiceDescription("Cacic Daemon");
    service.setStartupType(QtServiceController::AutoStartup);

    return service.exec();
}
