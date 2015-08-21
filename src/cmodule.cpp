#include "cmodule.h"

CModule::CModule()
{
}

int CModule::loadFromJsonObject(const QJsonObject &value)
{
    if (!value.isEmpty()){
        QString aux;
        aux = value["nome"].toString();
        if (!aux.isEmpty() && !aux.isNull()){
            this->_name = aux;
        } else {
            return ErrorCodes::InvalidName;
        }

        aux = value["hash"].toString();
        if (!aux.isEmpty() && !aux.isNull()){
            this->_hash = aux;
        } else {
            return ErrorCodes::InvalidHash;
        }

        this->_urlDownload = value["url"].toString();
        this->_version     = value["version"].toString();

        aux = value["type"].toString();
        if (!aux.isEmpty() && !aux.isNull()){
            if(aux.contains("http", Qt::CaseInsensitive)){
                this->_typeDownload = TypeDownload::Http;
            } else if (aux.contains("ftp", Qt::CaseInsensitive)){
                this->_typeDownload = TypeDownload::Ftp;
            } else if (!this->_urlDownload.isEmpty() &&
                       !this->_urlDownload.isNull()){
                if(this->_urlDownload.left(4).contains("http")){
                    this->_typeDownload = TypeDownload::Http;
                } else if (this->_urlDownload.left(3).contains("ftp")){
                    this->_typeDownload = TypeDownload::Ftp;
                }
            }
        }

        return ErrorCodes::Success;
    } else {
        return ErrorCodes::EmptyJson;
    }
}


QString CModule::name() const
{
    return _name;
}

void CModule::setName(const QString &name)
{
    _name = name;
}
QString CModule::hash() const
{
    return _hash;
}

void CModule::setHash(const QString &hash)
{
    _hash = hash;
}
QString CModule::urlDownload() const
{
    return _urlDownload;
}

void CModule::setUrlDownload(const QString &urlDownload)
{
    _urlDownload = urlDownload;
}
CModule::TypeDownload CModule::typeDownload() const
{
    return _typeDownload;
}

void CModule::setTypeDownload(const TypeDownload &typeDownload)
{
    _typeDownload = typeDownload;
}
QString CModule::version() const
{
    return _version;
}

void CModule::setVersion(const QString &version)
{
    _version = version;
}
