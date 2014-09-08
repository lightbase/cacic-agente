#ifndef LOGBROWSER_H
#define LOGBROWSER_H

#include <QDebug>
#include <QFile>
#include <QObject>

class LogFile : public QObject
{
    Q_OBJECT
public:
    explicit LogFile(QObject *parent = 0);
    explicit LogFile(const QString &fileLocation, QObject *parent = 0);
    ~LogFile();
    void outputMessage( QtMsgType type, const QString &msg );

private:
    QFile *logFile;

};

#endif // LOGBROWSER_H
