#include "ldaphandler.h"

LdapHandler::LdapHandler()
{
}

LdapHandler::LdapHandler(const QString &ldapServer)
{
    this->ldapServer = ldapServer;
    inicializar();
}

QString LdapHandler::busca(const QString &loginLdap,const QString &passLdap,const QString &baseLdap,const QString &filterLdap)
{
    // Método em teste
    // Estou verificando as funções chamadas pelo valor de rc e prints nos campos do formulário.

        char *login = (char*)loginLdap.toStdString().c_str();
        char *passwd = (char*)passLdap.toStdString().c_str();
        char *base = (char*)baseLdap.toStdString().c_str();
        char *filter = (char*)filterLdap.toStdString().c_str();
        ulong rc;

        ulong version = LDAP_VERSION3;
        rc = ldap_set_option(ldp, LDAP_OPT_PROTOCOL_VERSION, (void*)&version);
#if defined(Q_OS_UNIX)
        if ( rc != LDAP_OPT_SUCCESS){
            //error
        }
#elif defined(Q_OS_WIN)
        if ( rc != LDAP_SUCCESS){
            //error
        }
#endif

#if defined(Q_OS_UNIX)
        rc = ldap_simple_bind_s(ldp,login,passwd);
#elif defined(Q_OS_WIN)

//        LDAP_TIMEVAL *timeout = NULL;
//        rc = ldap_connect(ldp,(struct l_timeval*)timeout);
        rc = ldap_simple_bind_sA(ldp,login,passwd);
#endif
        if ( rc != LDAP_SUCCESS ){
            //error
        }

        char *attrs = NULL;
        LDAPMessage *res = NULL;
#if defined(Q_OS_UNIX)
        rc = ldap_search_s(ldp, base, LDAP_SCOPE_BASE,filter,(char**)&attrs,0,&res);
#elif defined(Q_OS_WIN)
        rc = ldap_search_sA(ldp, base, LDAP_SCOPE_BASE,filter,(char**)&attrs,0,&res);
#endif
        if ( rc != LDAP_SUCCESS ){
            //error
        }

}

bool LdapHandler::inicializar()
{
    char *host = (char*)ldapServer.toStdString().c_str();

#if defined(Q_OS_UNIX)
        rc = ldap_initialize( &ldp, host);
        if ( rc != LDAP_SUCCESS){
            //error
            return false;
        }
#elif defined(Q_OS_WIN)
        ldp = ldap_initA(host,389);
        if (ldp == NULL){
            //error
            return false;
        }
#endif
}

void LdapHandler::setServer(const QString &ldapServer)
{
    this->ldapServer = ldapServer;
}


