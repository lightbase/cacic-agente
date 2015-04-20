#include "cacicdeploy.h"

int main(int argc, char **argv)
{
    cacicdeploy service(argc, argv);
    service.setServiceDescription("Cacic Deploy");
    service.setStartupType(QtServiceController::AutoStartup);
    service.setServiceFlags(cacicdeploy::Default);

    return service.exec();
}
