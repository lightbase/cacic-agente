#include "installcacic.h"
#include "iostream"

InstallCacic::InstallCacic(QObject *parent) :
    QObject(parent)
{
}

void InstallCacic::run() {
    int code;
    std::cout << "Hello World\n";
    std::cin >> code;
    emit finished(code);
}


