#include "cacicd.h"

int main(int argc, char **argv)
{
    cacicD service(argc, argv);
    service.setServiceDescription(CACIC_SERVICE_NAME);
    service.setStartupType(QtServiceController::AutoStartup);
    service.setServiceFlags(cacicD::Default);

    return service.exec();
}
