Compilação:
	entre no diretorio do projeto e rode: make

Execução do servidor:
	entre no diretorio do projeto e rode: ./servidor <port>


Execução do cliente:
	1 Passo - Abra um novo terminal no computador onde o servidor esta rodando, e digite: ifconfig
	2 Passo - Procure por br0 -> inet end e copie um numero com o formato parecido ao: xxx.xx.x.xxx	
	3 Passo - Entre em outra maqui e digite o comando abaixo

	Em seguida entre no diretorio do projeto e rode: ./cliente <port> <endereço copiado no passo anterior>

	Rodando cliente na mesma maquina que o servidor: 
	ip = 127.0.0.1
	nome = loclahost
	rode: ./cliente <port> <ip/nome>


Para enviar ou pesquisar mensagens ao servidor, visite a documentação, procure no indice da documentação "Como salvar dados ?" ou "Como pesquisar dados?".



TABELA DE RETORNOS:
 0 - Retorno sem erros
 1 - Servidor ou cliente não respondeu
-1 - ERRO: Argumentos invalidos
-2 - ERRO: Falha na criação de processo
-3 - ERRO: Falha na apertura de arquivo
-3 - ERRO: Falha na criação do socket01
-4 - ERRO: Falha no bind
-5 - ERRO: Erro em envio de porta
