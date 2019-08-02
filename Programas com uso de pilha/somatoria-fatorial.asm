# Somatoria de n fatorial

# S0 = m
# S1 = somatoria do fatorial
main:

	addi $v0, $zero, 5
	
	syscall
	add $s0, $zero, $v0   # Salvando em s0 valor lido
	add $s1, $zero, $zero # zerando acumulador (somatoria)
	
	for:
		
		add $a0, $zero, $s0
		jal fatorial
		add $s1, $s1, $v0
				
		addi $s0, $s0, -1
	bne $s0, $zero, for	
	
	add $a0, $zero, $s1
	jal print
	j fim


fatorial:
	addi $sp, $sp, -8            # Alocando espaço para duas palavras
	sw $ra, 0($sp)               # Salva endereço de retorno
	sw $a0, 4($sp)               # Salva argumento a0
	slti $t0, $a0, 1             # Se a0 for menor que 3, T0 recebe 1, se a0 for maior que 3, T0 recebe 0
	beq $t0, $0, continue        # Se t0 for igual a zero, deve continuar pois o argumento não deve ser decrementado
	addi $sp, $sp, 8             # Restaura a pilha
	addi $v0, $0, 1              # Seta arumento de retorno
	jr $ra                       # Retorna valor
	
	continue:
		addi $a0, $a0, -1           # Decrementa o argumento em -1 e chama a função [FATORIAL] mais uma vez
		jal fatorial
		lw $ra, 0($sp)
		lw $a0, 4($sp)
		addi $sp, $sp, 8
		mul $v0, $a0, $v0
		jr $ra



print:
	addi $v0, $zero, 1
	syscall
	addi $a0, $0, 0xA #ascii code for LF, if you have any trouble try 0xD for CR.
  addi $v0, $0, 0xB 
  syscall
	jr $ra
	

fim: