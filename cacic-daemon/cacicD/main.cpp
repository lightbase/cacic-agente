#include <QCoreApplication>
#include "cacicd.h"
#include "cacictimer.h"

int main(int argc, char **argv)
{
    cacicD service(argc, argv);
    return service.exec();
}
