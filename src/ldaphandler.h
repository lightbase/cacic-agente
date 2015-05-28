#ifndef LDAPHANDLER_H
#define LDAPHANDLER_H

#include <QtCore>
#include <QDebug>
#include <iostream>

#define LDAP_DEPRECATED 1
//#undef UNICODE
//#undef LDAP_UNICODE
//#define LDAP_UNICODE 0
#if defined(Q_OS_UNIX)
//instalar pacote libldap2-dev
    #include <ldap.h>
#elif defined(Q_OS_WIN)
    #include <windows.h>
    #include <winldap.h>
#endif

class LdapHandler
{
public:
    LdapHandler();
    LdapHandler(const QString &ldapServer);

    QString busca(const QString &loginLdap,const QString &passLdap,const QString &baseLdap,const QString &filterLdap);
    bool inicializar();
    void setServer(const QString &ldapServer);

private:
    LDAP *ldp = NULL;
    QString ldapServer;
};

#endif // LDAPHANDLER_H
