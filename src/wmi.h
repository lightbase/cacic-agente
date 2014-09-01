#ifndef WMI_H
#define WMI_H

#include <ccacic.h>
#include <windows.h>
#include <ActiveQt/ActiveQt>

class wmi
{
public:
    wmi();
    static QJsonValue wmiSearch(QString classe, QStringList params);
private:
};

#endif // WMI_H
