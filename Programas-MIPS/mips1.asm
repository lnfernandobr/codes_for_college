# Leia um valor maior que 0 e imprimda a some de 1 até [N]
# Fernando Lima - 2019
# UEMS - Ciências da computação


# Leia [N] e coloque no registrador de variavel $s0
addi $v0, $zero, 5 # Seta 5 para serviço de leitura, valor lido ficará em $v0
syscall
add $s0, $zero, $v0 # Coloca o valor lido em $v0 no registrador $s0

addi $t0, $zero, 0 # Zera o registrador $t0 para utiliza-lo como acumulador

# do {

# } while();

do: 
	add $t0, $t0, $s0  # t0 += N;
	addi $s0, $s0, -1  # N--;
	bne $s0, $zero, do # Se o valor do registrador $s0 for diferente de zero, pule para o label do:
# FIM DO DO WHILE


addi $v0, $zero, 1   # Seta serviço de impressao stdout
add $a0, $zero, $t0  # Coloca valor da operação realizada no laço do no registrador de impressao $s0
syscall              # Chama o sistema




	

