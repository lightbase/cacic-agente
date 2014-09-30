#include <QCoreApplication>
#include "cacicd.h"

int main(int argc, char **argv)
{
    cacicD service(argc, argv);

    return service.exec();
}
