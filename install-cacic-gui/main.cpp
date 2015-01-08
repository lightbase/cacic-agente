#include "installcacicgui.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    InstallCacicGui w;
    w.show();

    return a.exec();
}
