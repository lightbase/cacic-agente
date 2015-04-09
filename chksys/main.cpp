#include "chksys.h"

int main(int argc, char **argv)
{
    chksys service(argc, argv);
    service.setServiceDescription("Check Cacic");
    service.setStartupType(QtServiceController::AutoStartup);
    service.setServiceFlags(chksys::Default);

    return service.exec();
}
