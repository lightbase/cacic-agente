#include <QStringList>
#include <QDir>
#include <QSettings>
#include "qtservice.h"
int processArgs(int argc, char **argv)
{
    if (argc > 1) {
        QString arg1(argv[1]);
        if (arg1 == QLatin1String("-i") ||
                arg1 == QLatin1String("-install")) {

            QString path = "./cacic-service";
            QFile daemonExec(path);
            if( !daemonExec.exists() ) {
                qCritical("Executável do serviço/daemon não encontrado.");
                return 0;
            }

            if (argc > 1) {
                QString account;
                QString password;

                if (argc > 2)
                    account = argv[2];
                if (argc > 3)
                    password = argv[3];
                printf("O serviço %s instalado.\n",
                       (QtServiceController::install(path, account, password) ? "foi" : "não foi"));
                return 0;
            }
        } else {
            QtServiceController controller("CacicDaemon");
            QString option(argv[1]);
            if (option == QLatin1String("-u") ||
                    option == QLatin1String("-uninstall")) {
                printf("The service \"%s\" %s uninstalled.\n",
                    controller.serviceName().toLatin1().constData(),
                    (controller.uninstall() ? "was" : "was not"));
                return 0;
            } else if (option == QLatin1String("-s") ||
                       option == QLatin1String("-start")) {
                QStringList args;
                for (int i = 2; i < argc; ++i)
                    args.append(QString::fromLocal8Bit(argv[i]));
                printf("O serviço \"%s\" %s iniciado.\n",
                       controller.serviceName().toLatin1().constData(),
                       (controller.start(args) ? "foi" : "não foi"));
                return 0;
            } else if (option == QLatin1String("-t") ||
                       option == QLatin1String("-terminate")) {
                printf("O serviço \"%s\" %s terminado.\n",
                       controller.serviceName().toLatin1().constData(),
                       (controller.stop() ? "foi" : "não foi"));
                return 0;
            } else if (option == QLatin1String("-p") ||
                       option == QLatin1String("-pause")) {
                printf("O serviço \"%s\" %s pausado.\n",
                       controller.serviceName().toLatin1().constData(),
                       (controller.pause() ? "foi" : "não foi"));
                return 0;
            } else if (option == QLatin1String("-r") ||
                       option == QLatin1String("-resume")) {
                printf("O serviço \"%s\" %s resumido.\n",
                       controller.serviceName().toLatin1().constData(),
                       (controller.resume() ? "foi" : "não foi"));
                return 0;
            } else if (option == QLatin1String("-c") ||
                       option == QLatin1String("-command")) {
                if (argc > 2) {
                    QString codestr(argv[2]);
                    int code = codestr.toInt();
                    printf("O comando %s enviado para o serviço \"%s\".\n",
                           (controller.sendCommand(code) ? "foi" : "não foi"),
                           controller.serviceName().toLatin1().constData());
                    return 0;
                }
            } else if (option == QLatin1String("-v") ||
                       option == QLatin1String("-version")) {
                bool installed = controller.isInstalled();
                printf("O serviço\n"
                       "\t\"%s\"\n\n", controller.serviceName().toLatin1().constData());
                printf("está %s", (installed ? "instalado" : "não instalado"));
                printf(" e %s\n\n", (controller.isRunning() ? "executando" : "não executando"));
                if (installed) {
                    printf("caminho: %s\n", controller.serviceFilePath().toLatin1().data());
                    printf("descrição: %s\n", controller.serviceDescription().toLatin1().data());
                    printf("inicialização: %s\n", controller.startupType() == QtServiceController::AutoStartup ? "Auto" : "Manual");
                }
                return 0;
            }
        }
    }
    printf("cacic-daemon [-i | -v | -u | -s | -t | -p | -r | -c CODE | -h] [-w]\n\n"
           "\t-i(nstall)\t: Instala o serviço\n"
           "\t-v(ersion)\t: Imprime status do serviço\n"
           "\t-u(ninstall)\t: Desinstala o serviço\n"
           "\t-s(tart)\t: Inicia o serviço\n"
           "\t-t(erminate)\t: Termina o serviço\n"
           "\t-p(ause)\t: Pausa o serviço\n"
           "\t-r(esume)\t: Resume o serviço\n"
           "\t-c(ommand) CODE\t: Envia um comando CODE para o serviço\n"
           "\t-h(elp)\t\t: Imprime este menu de ajuda\n"
           "\t-w(ait)\t\t: Aguarda uma tecla ser pressionada ao terminar\n");
    return 0;
}
int main(int argc, char **argv)
{

#if !defined(Q_OS_WIN)
    // QtService stores service settings in SystemScope, which normally require root privileges.
    // To allow testing this example as non-root, we change the directory of the SystemScope settings file.
    QSettings::setPath(QSettings::NativeFormat, QSettings::SystemScope, QDir::tempPath());
    qWarning("(Example uses dummy settings file: %s/QtSoftware.conf)", QDir::tempPath().toLatin1().constData());
#endif

    int result = processArgs(argc, argv);
    if (QString::fromLocal8Bit(argv[argc-1]) == QLatin1String("-w") ||
            QString::fromLocal8Bit(argv[argc-1]) == QLatin1String("-wait")) {
        printf("\nAperte Enter para continuar...");
        QFile input;
        input.open(stdin, QIODevice::ReadOnly);
        input.readLine();
        printf("\n");
    }
    return result;
}
