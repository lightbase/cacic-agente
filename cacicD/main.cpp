#include <QCoreApplication>
#include "cacicd.h"
#include "cacictimer.h"

int main(int argc, char **argv)
{
    cacicD sevice(argc, argv);
    return sevice.exec();
}
