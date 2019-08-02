# Exercicio 1 - treino para prova T(n) = 2T(piso(n/2)) + n

# S1 = 2
# S2 = Resposta

main:
	addi $v0, $zero, 5
	syscall
	add $a0, $zero, $v0
	jal T
	add $a0, $zero, $v0
	add $v0, $zero, 1
	syscall
	j fim
	
T:
	add $sp, $sp, -8
	sw $ra, 0($sp)
	sw $a0, 4($sp)
	add $v0, $zero, 1
	slti $t0, $a0, 2
	beq $t0, $zero, continue
	add $sp, $sp, 8
	jr $ra
	
	continue:
	add $s1, $zero, 2
	div $a0, $s1
	mflo $a0
	jal T
	lw $ra, 0($sp)
	lw $a0, 4($sp)
	add $sp, $sp, 8
	mult $v0, $s1
	mflo $s2
	add $v0, $a0, $s2
	jr $ra
		
fim:

