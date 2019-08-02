# Bits trocados:
# As Ilhas Weblands formam um reino independente nos mares do Pacífico. Como é um reino
# recente, a sociedade é muito influenciada pela informática. A moeda oficial é o Bit; existem
# notas de B$ 50,00, B$10,00, B$5,00 e B$1,00. Você foi contratado(a) para ajudar na
# programação dos caixas automáticos de um grande banco das Ilhas Weblands.
# Os caixas eletrônicos das Ilhas Weblands operam com todos os tipos de notas disponíveis,
# mantendo um estoque de cédulas para cada valor (B$ 50,00, B$10,00, B$5,00 e B$1,00). Os
# clientes do banco utilizam os caixas eletrônicos para efetuar retiradas de um certo número
# inteiro de Bits.
# Sua tarefa é escrever um programa MIPS que, dado o valor de Bits desejado pelo cliente,
# determine o número de cada uma das notas necessário para totalizar esse valor, de modo a
# minimizar a quantidade de cédulas entregues. Por exemplo, se o cliente deseja retirar
# B$50,00, basta entregar uma única nota de cinquenta Bits. Se o cliente deseja retirar
# B$72,00, é necessário entregar uma nota de B$50,00, duas de B$10,00 e duas de B$1,00.

# int main(){
#	 int notas[4] = {50, 10, 5, 1}, res[4];
#  int i, valor, valores = 0;
#	 scanf("%i", &valor);
#
#	 valores++;
#
#	 for(i = 0; i < 4; i++) {
#		 res[i] = valor / notas[i];
#		 valor = valor % notas[i];
#	 }
#	 printf("%i %i %i %i\n", valores, res[0], res[1], res[2], res[3]);
# }

# S0 = 50
# S1 = 10
# S2 = 5
# S3 = 1
# S4 = valor
# S5 = valores

# SEM USO DE VETORES 
main:
	addi $s0, $zero, 50
	addi $s1, $zero, 10
	addi $s2, $zero, 5
	addi $s3, $zero, 1

	addi $v0, $zero, 5
	syscall
	
	add $s4, $zero, $v0 # Salvando entrada do usuario em $s4
	add $s5, $zero, $zero # zerando variavel valores
	
	div $s4, $s0
	mflo $t0
	mfhi $s4
	add $t1, $zero, $t0
	
	div $s4, $s1
	mflo $t0
	mfhi $s4
	add $t2, $zero, $t0
	
	div $s4, $s2
	mflo $t0
	mfhi $s4
	add $t3, $zero, $t0
	
	div $s4, $s3
	mflo $t0
	mfhi $s4
	add $t4, $zero, $t0
	
	add $a0, $zero, $t1
	jal print
	
	add $a0, $zero, $t2
	jal print

	add $a0, $zero, $t3
	jal print

	add $a0, $zero, $t4
	jal print

	j fim
	
print:
	addi $v0, $zero, 1
	syscall
	jr $ra

fim:
	
	