Módulo agente do software Cacic
============
Pré Requisitos:
- Crypto++ (http://www.cryptopp.com/#download)

Instalando no Windows:
>Caso dê problemas durante a compilação e for solicitado o "this->" antes de alguns métodos, deve ser corrigido em cada arquivo. Depois disso basta seguir esse tutorial: https://sites.google.com/site/ievgensychov/cryptopp

Instalando no Linux (Versão utilizada Ubuntu 14.04):

    1 ~$ apt-cache pkgnames | grep -i crypto++
  
    libcrypto++X-dbg
    libcrypto++-dev
    libcrypto++-doc
    libcrypto++X
    libcrypto++-utils
  
    *O X é referente a versão do cryptopp. Modifique de acordo com o resultado da busca.
  
    2 ~$ apt-get install libcrypto++X libcrypto++X-dbg libcrypto++-dev

- OpenSSL (https://www.openssl.org/)
>Encontrado para windows em https://www.openssl.org/related/binaries.html;


