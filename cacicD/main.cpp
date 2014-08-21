#include <QCoreApplication>
#include "cacicd.h"

int main(int argc, char **argv)
{
    cacicD sevice(argc, argv);
    return sevice.exec();
}
