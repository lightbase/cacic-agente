#ifndef WMI_H
#define WMI_H

#include <ccacic.h>
//#include <logcacic.h>
//#include <QObject>
#ifdef Q_OS_WIN
#include <windows.h>
#include <ActiveQt/ActiveQt>
#endif

class wmi //: QObject
{
//    Q_OBJECT
public:
    wmi();
    static QJsonValue wmiSearch(QString classe, QStringList params);
//slots private:
//    void logException(int code, const QString &source, const QString &desc, const QString &help);
};

#endif // WMI_H
