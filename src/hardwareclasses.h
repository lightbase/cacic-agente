#ifndef HARDWARECLASSES_H
#define HARDWARECLASSES_H

#include <QString>
#include <QStringList>
#include <QHash>
#include <QDebug>

class HardwareClasses
{
public:
    HardwareClasses();
    HardwareClasses(const QHash<QString, QStringList> &exceptions);
    bool init();
    void addClass(const QString &name, const QStringList &atr = QStringList());
    QHash<QString, QStringList> classes() const;
private:
    QHash<QString, QStringList> _classes;
    QHash<QString, QStringList> _exceptionClasses;
    bool _initialized;
};

#endif // HARDWARECLASSES_H
