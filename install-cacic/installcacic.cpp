#include "installcacic.h"
#include "iostream"

InstallCacic::InstallCacic(QObject *parent) :
    QObject(parent)
{
}

void InstallCacic::run() {
    std::cout << "Hello World\n";
    emit finished();
}
