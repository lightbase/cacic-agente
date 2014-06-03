  Atualmente o agente cacic funciona a partir de um serviço do windows, que assim que é iniciado realiza algumas verificações de diretório e consistência dos módulos e inicia o módulo principal, o cacic280, que, quando executado, realiza a chamada do módulo gercols com o parâmetro getConfigs para pegar as configurações do gerente, em seguida o chama de novo para a coleta. A coleta de hardware, assim como IP, nome do computador e etc, é realizada por WMI e a de software por meio dos registros do windows (regedit).

  Durante a pesquisa realizada do OCSInventory, notei que há muito que possamos melhorar, tanto em coleta quanto em desempenho.

  A principal mudança que vi foi na coleta, pois além do WMI, podemos fazer também pelo registro. Isso reduz a margem de erros durante a coleta. Além disso, a coleta pode ser feita de forma bem mais organizada.

  Então, esse agente basicamente virá para melhorar, obviamente. A princípio pensei em algo bem parecido na teoria:

#**O início de tudo, Instalador:**
  O Instalador para Windows acredito que a melhor maneira seria fazer um MSI. Reduz a chance de conflito com firewall e etc, ficando também mais amigável pro usuário, já que a maioria dos instaladores são parecidos.
Tentei pensar em algo menos trabalhoso, pois quando instalado por meio do netlogon pode complicar pra instalar na rede, como já presenciamos na PGFN, e também atrasar o login do usuário, então pensei nos seguintes procedimentos:
  * No windows: Para rodar no netlogon deverá ser passado como parâmetro o servidor para autenticação e o '/silent' pra rodar em segundo plano. Caso não seja netlogon, o usuário irá digitar o servidor em uma das telas de diálogo;
  * O instalador realiza o getTest;
  * Se positivo, cria a árvore de diretórios e faz download do módulo principal e a biblioteca de comunicação (dll ou so);
  * Cria o serviço para o módulo e o inicia.

#**O módulo principal.**
  * Após a instalação, o módulo principal seria iniciado, coletando as informações de configuração (getConfig) e baixando as bibliotecas necessárias, que seriam as .dll (windows) ou .so(linux), para realizar a coleta depois;
  * Faz a verificação das bibliotecas;
  * Inicializa a biblioteca e realiza a coleta;
  * Se for a primeira coleta ou existir diferença entre a coleta atual e a antiga, os dados serão enviados ao gerente a princípio pelo mesmo formato de XML do antigo (mas será modificado em uma nova release para json ou algo parecido);

##*A coleta*
  Na instalação poderia ser setado um horário randomico entre 8 e 18 horas pra realizar a coleta diária e, também, pra não fazer todos rodarem ao mesmo tempo e congestionar o gerente. Caso o computador não esteja logado no horário estabelecido, a coleta será feita assim que for ligado.
Nessa "rotina diária" seria realizado:

  * O procedimento de checagem das bibliotecas e etc, pra caso alguma esteja faltando ser realizado o download de novo;
  * O getConfig;
  * As coletas;

##*Força Coleta*
  Aqui vem minha maior dúvida, estava pensando em algum tipo de sincronia em tempo real. Estou dando uma pesquisada ainda, mas isso demandaria uma atualização também no gerente. Então a princípio ficaria o mesmo, seria realizado por meio do getTest de 5 em 5 minutos. Se houver alguma atualização, é realizado o getConfig e em seguida o procedimento solicitado, como forçar coleta ou executar o mapa.
