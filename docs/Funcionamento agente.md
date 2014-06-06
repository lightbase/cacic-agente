  Atualmente o agente cacic funciona a partir de um serviço do windows, que assim que é iniciado realiza algumas verificações de diretório e consistência dos módulos e inicia o módulo principal, o cacic280, que, quando executado, realiza a chamada do módulo gercols com o parâmetro getConfigs para pegar as configurações do gerente, em seguida o chama de novo para a coleta. A coleta de hardware, assim como IP, nome do computador e etc, é realizada por WMI e a de software por meio dos registros do windows (regedit).

  Durante a pesquisa realizada do OCSInventory, notei que há muito que possamos melhorar, tanto em coleta quanto em desempenho.

  A principal mudança que vi foi na coleta, pois além do WMI, podemos fazer também pelo registro. Isso reduz a margem de erros durante a coleta. Além disso, a coleta pode ser feita de forma bem mais organizada.

  A princípio pensei em algo bem parecido, até porque para maiores modificações o gerente também deve ser modificado, o que não ocorrerá por agora.


#**Cacic Service**
  O cacic service funcionará apenas de sustentação do módulo principal, verificando de tempos em tempos a consistência dos binários e executando o módulo principal.
  Como funcionará:
  * Durante a instalação do agente, será realizada a instalação do serviço;
  * A cada 5 minutos será solicitado ao gerente algumas informações básicas (getTest), caso, dentro dessas informações, o gerente solicitar uma coleta forçada o módulo principal será executado;
  * O módulo a cada X horas chamará o módulo principal, sendo X o tempo estabelecido pelo gerente.

#**O módulo principal.**
  * O módulo principal será chamado pelo serviço do cacic;
  * Solicitará informações de configurações do gerente (getConfig);
  * Faz a verificação das bibliotecas e dos módulos e, se necessário, realiza o download via ftp;
  * Inicializa o módulo de coleta (gercols);
  * É finalizado.

PS: Há também a coleta da informação de quando um USB for plugado ou retirado. Estava verificando, há uma função no WMI que o próprio windows chama um executável especificado quando algo acontece no usb (http://msdn.microsoft.com/en-us/library/windows/desktop/aa363480%28v=vs.85%29.aspx). Mas estou verificando uma forma mais segura e multiplataforma.

##*A coleta*
  A coleta é realizada a cada X horas, onde X é o valor estabelecido pelo gerente. O módulo principal será chamado dentro desse horário, chamando o módulo de coletas que coletará as informações de hardware, pelo WMI ou, caso não consiga, pelos registros (regedit), e também de software pelos registros (regedit).
  Então, resumidamente, nessa rotina seria realizado:

  * O getConfig (solicitação das informações de configurações);
  * A coleta de hardware por WMI, caso não seja possível é realizada a tentativa pelo regedit;
  * A coleta de software pelo regedit;
  * O envio das coletas é realizado separadamente, um envio pra software, um pra hardware, etc.;
  * Se não houver coletas anteriores ou a coleta atual for diferente da anterior, o envio das coletas para o gerente.


##*Força Coleta*
  Aqui vem minha maior dúvida, estava pensando em algum tipo de sincronia em tempo real. Estou dando uma pesquisada ainda, mas isso demandaria uma atualização também no gerente. Então a princípio ficaria o mesmo, seria realizado por meio de uma solicitação do cacicService de getTest de 5 em 5 minutos. Se houver alguma atualização, é chamado o módulo principal executando o procedimento solicitado pelo gerente, como forçar coleta ou executar o mapa.

#**O início de tudo, Instalador:**
  O Instalador para Windows acredito que a melhor maneira seria fazer um MSI. Reduz a chance de conflito com firewall e etc, ficando também mais amigável pro usuário, já que a maioria dos instaladores são parecidos.
Tentei pensar em algo menos trabalhoso, pois quando instalado por meio do netlogon pode complicar pra instalar na rede, como já presenciamos na PGFN, e também atrasar o login do usuário, então pensei nos seguintes procedimentos:
  * No windows: Para rodar no netlogon deverá ser passado como parâmetro o servidor para autenticação e o '/silent' pra rodar em segundo plano. Caso não seja netlogon, o usuário irá digitar o servidor em uma das telas de diálogo;
  * O instalador realiza o getTest;
  * Se positivo, cria a árvore de diretórios e faz download do cacicService, do módulo principal e a biblioteca de comunicação (dll ou so);
  * Cria o serviço no windows e o inicia.
