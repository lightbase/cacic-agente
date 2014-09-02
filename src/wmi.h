#ifndef WMI_H
#define WMI_H

#include <ccacic.h>
#ifdef Q_OS_WIN
#include <windows.h>
#include <ActiveQt/ActiveQt>
#endif

class wmi
{
public:
    wmi();
    static QJsonValue wmiSearch(QString classe, QStringList params);
private:
};

#endif // WMI_H
