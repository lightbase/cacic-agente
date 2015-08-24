#ifndef CMODULEARRAY_H
#define CMODULEARRAY_H

#include <QString>
#include <QJsonArray>
#include <QJsonObject>
#include <QJsonValue>
#include <cmodule.h>
#include <QVector>
#include <QDebug>

class CModuleArray
{
public:
    /**
     * @brief The ErrorCodes enum
     */
    enum ErrorCodes{
        Success     =  0,
        EmptyJson   = -1,
    };

    CModuleArray();
    /**
     * @brief loadFromJsonArray
     * @param json
     * @return
     */
    ErrorCodes loadFromJsonArray(const QJsonArray &json);
    /**
     * @brief size
     * @return return the numer of CModule
     */
    int size() const;
    /**
     * @brief getError
     * @return
     */
    int getError() const;
    /**
     * @brief CModuleArray::operator []
     * @param i
     * @return
     */
    CModule & operator[](int i);

private:
    /**
     * @brief _size
     */
    int _size;
    /**
     * @brief _error
     */
    int _error;
    /**
     * @brief _modules<CModule>
     */
    CModule *_modules;

};

#endif // CMODULEARRAY_H
