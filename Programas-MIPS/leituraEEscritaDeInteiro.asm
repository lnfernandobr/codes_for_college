addi $v0,$zero,5	#carrega serviço de leitura de inteiros
syscall					  #chama serviço de leitura
add $a0,$zero,$v0 #move resultado da leitura para $a0
addi $v0,$zero,1	#carrega serviço de impressão de inteiros
syscall						#chama serviço de impressão (p/$a0)
