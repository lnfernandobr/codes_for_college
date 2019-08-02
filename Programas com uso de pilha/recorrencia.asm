# S0 = n


main:
	add $s0, $0, 4 # guarda em s0 o valor de n
	add $a0, $zero, $s0
	jal T
	add $a0, $zero, $v0
	add $v0, $zero, 1
	syscall
	j exit
	

# a0 = n
T:
	add $t0, $zero, 2
	add $sp, $sp, -8
	sw $ra, 0($sp) 						# Salva endereço de retorno na pilha
	sw $a0, 4($sp) 						# Salva argumento na pilha, pois sera alterado conforme a função é executada
	slti $t2, $a0, 2					# Compara  a0 < 2, se sim seta em t2 1, se a1 for maior
	beq $t2, $zero, continue  # Pula para o label continue se a0 é maior que 1
	addi $v0, $zero, 1        # Seta argumento de retorno 1, caso o n seja menor que 2
	add $sp, $sp, 8           # Restaura a pilha
	jr $ra										# Retorna
		
	continue:
		div $a0, $t0 # Devide argumento por 2
		mflo $a0   # Coloca resultado do retorno em a0 para passar como argumento
		jal T      # Chama a função recursiva
		lw $a0, 4($sp)		  # Carrega a0 da pilha
		lw $ra, 0($sp)			# Carrega ra (retorno) da pilha
		
		add $s4, $zero, $v0 # s4 = v0
		mul $v0, $s4, $t0 	# s4 * 2
		add $v0, $v0, $a0   # V0 = v0 + 2

		add $sp, $sp, 8     # Restaura pilha
		jr $ra
	
exit: