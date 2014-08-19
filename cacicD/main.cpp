#include <QCoreApplication>
#include <QStringList>
#include <QDir>
#include <QSettings>
#include <qtservice/src/qtservice.h>

int main(int argc, char **argv)
{
#if !defined(Q_OS_WIN)
    QSettings::setPath(QSettings::NativeFormat, QSettings::SystemScope, QDir::tempPath());
    qWarning("(Example uses dummy settings file: %s/QtSoftware.conf)", QDir::tempPath().toLatin1().constData());
#endif
    //int result = processArgs(argc, argv);
    if (QString::fromLocal8Bit(argv[argc-1]) == QLatin1String("-w") ||
            QString::fromLocal8Bit(argv[argc-1]) == QLatin1String("-wait")) {
        printf("\nPress Enter to continue...");
        QFile input;
        input.open(stdin, QIODevice::ReadOnly);
        input.readLine();
        printf("\n");
    }
    //return result;
}


