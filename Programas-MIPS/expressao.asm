# Função para mostrar numeros pares ou numeros impares. 
# argumento 1 impar argumento 0 par, argumento n para
# imprimir pares ou impares no intervalo



main:
add $a0, $zero, 0
add $a1, $zero, 10
jal parImpar
j exit



parImpar:

	add $s0, $zero, 2
	add $s1, $zero, $a0
	for:
		
	beq $s1, $zero, par
	
	impar:
		# imprime impar  
		div $a1, $s0
		mfhi $t0
		beq $t0, $zero, continuar
		add $v0, $zero, 1
		add $a0, $zero, $a1
		syscall
		j continuar
		
	par:
		div $a1, $s0
		mfhi $t0
		bne $t0, $zero, continuar
		add $v0, $zero, 1
		add $a0, $zero, $a1
		syscall

	continuar:

	addi $a1, $a1, -1
	bne $a1, $zero, for

jr $ra



exit: