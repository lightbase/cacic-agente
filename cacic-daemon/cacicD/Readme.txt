Para instalar um serviço no Windows:
	~$ sc create "CacicD" binPath= "C:\endereco\do\cacicd\CacicD.exe" start= auto

Para instalar um serviço no Linux (TESTADO no Ubuntu 14.04):
	1- copiar o /etc/init.d/skeleton para /etc/init.d/cacic e fazer as modificações necessarias.
	   (como por exemplo o endereço do binario) e dar as permissões de execução.
		* edit as linhas: DESC="Description of the service" //descrição
				  NAME=daemonexecutablename //nome correto do binario
				  DAEMON=/usr/sbin/$NAME // endereço/correto/do/binario/$NAME
		* sudo chmod +x cacic
	2- criar um link simbolico para /etc/rc2.d/S99cacic
		* sudo ln -s /etc/init.d/cacic /etc/rc2.d/S99cacic
	3- iniciar o serviço.
		* /bin/bash -vx /etc/init.d/cacic start
