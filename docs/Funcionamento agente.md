  Atualmente o agente cacic funciona a partir de um serviço do windows, que assim que é iniciado realiza algumas verificações de diretório e consistência dos módulos e inicia o módulo principal, o cacic280, que, quando executado, realiza a chamada do módulo gercols com o parâmetro getConfigs para pegar as configurações do gerente, em seguida o chama de novo para a coleta. A coleta de hardware, assim como IP, nome do computador e etc, é realizada por WMI e a de software por meio dos registros do windows (regedit).

  Durante a pesquisa realizada do OCSInventory, notei que há muito que possamos melhorar, tanto em coleta quanto em desempenho.

  A principal mudança que vi foi na coleta, pois além do WMI, podemos fazer também pelo registro. Isso reduz a margem de erros durante a coleta. Além disso, a coleta pode ser feita de forma bem mais organizada.

  Então, esse agente basicamente virá para melhorar, obviamente. A princípio pensei em algo bem parecido na teoria:

#**O início de tudo, Instalador:**
  O Instalador para Windows acredito que a melhor maneira seria fazer um MSI. Reduz a chance de conflito com firewall e etc, ficando também mais amigável pro usuário, já que a maioria dos instaladores são parecidos.
Tentei pensar em algo menos trabalhoso, pois quando instalado por meio do netlogon pode complicar pra instalar na rede, como já presenciamos na PGFN, e também atrasar o login do usuário, então pensei nos seguintes procedimentos:
  * No windows: Para rodar no netlogon deverá ser passado como parâmetro o servidor para autenticação e o '/silent' pra rodar em segundo plano. Caso não seja netlogon, o usuário irá digitar o servidor em uma das telas de diálogo;
  * O instalador realiza o getTest;
  * Se positivo, cria a árvore de diretórios e faz download do cacicService, do módulo principal e a biblioteca de comunicação (dll ou so);
  * Cria o serviço no windows e o inicia.

#**Cacic Service**
  O cacic service funcionará apenas de sustentação do módulo principal, verificando de tempos em tempos a consistência dos binários e executando o módulo principal.

#**O módulo principal.**
  * Após a instalação, o módulo principal seria iniciado, coletando as informações de configuração (getConfig) e baixando as bibliotecas, que seriam as .dll (windows) ou .so(linux), e módulos exigidos pelo gerente;
  * Faz a verificação das bibliotecas;
  * Inicializa o módulo de coleta (gercols);
  * Se for a primeira coleta ou existir diferença entre a coleta atual e a antiga, os dados serão enviados ao gerente a princípio pelo mesmo formato de XML do antigo (mas será modificado em uma nova release para json ou algo parecido);

##*A coleta*
  A coleta é realizada a cada X horas, onde X é o valor estabelecido pelo gerente. O módulo principal será chamado dentro desse horário, chamando o módulo de coletas que coletará as informações de hardware pelo WMI ou, caso não consiga, pelos registros (regedit) e também de software pelos registros (regedit).
  Então, resumidamente, nessa "rotina diária" seria realizado:

  * O procedimento de checagem das bibliotecas e etc, pra caso alguma esteja faltando ser realizado o download de novo;
  * O getConfig;
  * A chamada do módulo de coletas;
  * As coletas;
  * O envio das informações, se existir diferença entre a coleta atual e a antiga. Os dados serão enviados ao gerente a princípio pelo mesmo formato de XML do antigo (mas será modificado em uma nova release para json ou algo parecido);

##*Força Coleta*
  Aqui vem minha maior dúvida, estava pensando em algum tipo de sincronia em tempo real. Estou dando uma pesquisada ainda, mas isso demandaria uma atualização também no gerente. Então a princípio ficaria o mesmo, seria realizado por meio de uma solicitação do cacicService de getTest de 5 em 5 minutos. Se houver alguma atualização, é chamado o módulo principal executando o procedimento solicitado pelo gerente, como forçar coleta ou executar o mapa.
