#include "cmodulearray.h"

CModuleArray::CModuleArray()
{
}

CModuleArray::ErrorCodes CModuleArray::loadFromJsonArray(const QJsonArray &json)
{
    if (json.size() > 0){
        int i = json.size();
        _modules = new CModule[i];

        i = 0;
        foreach(QJsonValue value, json){
            CModule module;
            int error = module.loadFromJsonObject(value.toObject());
            if (error == 0 ){
//                qDebug() << module.name();
                _modules[i] = module;
                i++;
            } else if (error == CModule::EmptyJson) {
//                qDebug() << "JSON VAZIO";
            } else if (error == CModule::InvalidHash) {
//                qDebug() << "INVALID HASH";
            } else if (error == CModule::InvalidName) {
//                qDebug() << "INVALID NAME";
            } else {
//                qDebug() << "wtf";
            }
        }
        this->_size = i;
        return ErrorCodes::Success;
    } else {
        this->_error = ErrorCodes::EmptyJson;
        return ErrorCodes::EmptyJson;
    }
}
int CModuleArray::size() const
{
    return _size;
}

int CModuleArray::getError() const
{
    return this->_error;
}

CModule &CModuleArray::CModuleArray::operator[](int i)
{
    return this->_modules[i];
}







