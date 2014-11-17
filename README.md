Módulo agente do software Cacic
============
Pré Requisitos:
- Crypto++ (http://www.cryptopp.com/#download)

Instalando no Windows:
>https://sites.google.com/site/ievgensychov/cryptopp
>Caso dê problemas durante a compilação e for solicitado o "this->" antes de alguns métodos, deve ser corrigido em cada arquivo. Depois disso basta o tutorial.
>Após a compilação, colocar no diretório src/crypto++/lib e modificar o path de onde está o seu projeto em todos os .pro na linha que a libcript é importada.

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

Instalação do agente:
>No windows, deve colocar as dll's que está dentro do zip (as dll's são as do QT, se o QT estiver instalado, não há necessidade) em uma pasta qualquer e adcionar a pasta às variáveis de ambiente. (Botão auxiliar em 'Meu Computador', navegue até 'Opções Avançadas' e 'Variáveis de Ambientes'. Procure a chave 'path' e coloque o diretório onde estão as dll's no final, separado por ;).

>No linux, primeiramente deve ativar a opção 'universe' em Softwares & Updates (nas configurações do sistema). Caso seja o Ubuntu 12, deve adicionar o repositório ppa:ubuntu-sdk-team/ppa (basta executar o comando 'sudo add-apt-repository ppa:ubuntu-sdk-team/ppa & sudo apt-get update'). Depois de configurar, basta executar o .deb e, ao final da instalação, no terminal, executar 'install-cacic -host=ip_do_gerente -user=usuario -pass=senha'.
