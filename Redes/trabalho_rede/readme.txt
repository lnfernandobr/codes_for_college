Compilação:
	entre no diretorio do projeto e rode: make


Execução do servidor:
	entre no diretorio do projeto e rode: ./servidor <port>


Execução do cliente:
	1 Passo - Abra um novo terminal e digite: ifconfig
	2 Passo - 	
	3 Passo - Se não conseguiu encontrar o endereço no 2 Passo, procure por endereço de LoopBack e copie.

	Em seguida entre no diretorio do projeto e rode: ./cliente <port> <endereço copiado no passo anterior>


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
