#include <QCoreApplication>
#include <QStringList>
#include <iostream>

using namespace std;

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
    QString argumento;
    cout << "Testando argumentos" << endl;
    if (a.arguments().count()>1){
        for(int i=1;i<a.arguments().count();i++){
            argumento = "--host";
            if (a.arguments().at(i) == argumento){
                return 5;
            } else {
                cout << a.arguments().at(i).toStdString() << endl;
            }
        }
    }

    return 1;
//    return a.exec();
}
