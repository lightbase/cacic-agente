#ifndef CONSOLE_H
#define CONSOLE_H

#include <QDebug>
#include <QString>

class ConsoleObject:public std::unary_function<std::string,QString>
{

public:
    QString operator() (std::string)
    {
        return "";
    }
};

#endif // CONSOLE_H
