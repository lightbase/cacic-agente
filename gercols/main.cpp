#include <QCoreApplication>
#include <gercols.h>

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
    //crio o objeto que será responsável pela execução
    Gercols *oGercols = new Gercols(&a);
    //conecto o sinal "finished" do objeto oGercols ao slot "quit" do objeto a(QCoreApplication)
    //ou seja, quando o finished for chamado, a aplicacao é encerrada.
    a.connect(oGercols, SIGNAL(finished()), &a, SLOT(quit()));
    //invoco o metodo "run" do objeto oGercols assim que o loop (a.exec()) comecar
    QMetaObject::invokeMethod(oGercols, "run", Qt::QueuedConnection);
    return a.exec();
}
