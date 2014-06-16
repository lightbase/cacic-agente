#include <installcacic.cpp>

using namespace std;

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
    InstallCacic CInstallCacic;
    CInstallCacic.setApp(*a);

    try {

       CInstallCacic.runInstall();// TENTAR RODAR COM CONNECT OU ALGO DO TIPO
    } catch (int e) {
        cout << "Error: " << e << endl;
    }

    return a.exec();
}
