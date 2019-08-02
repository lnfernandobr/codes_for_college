# Somatoria até n de (i * r^i) i variando de 0 a n

main:
	add $v0, $zero, 5			 	# Seta serviço de leitura
	syscall
	add $s0, $zero, $v0  		# S0 = n

	add $v0, $zero, 5			 	# Seta serviço de leitura
	syscall
	add $s1, $zero, $v0  		# S1 = n
	
	addi $t0, $zero, 0 			# Seta 0 no acumulador
	add $s3, $zero, $zero   # Seta 0 para a somatoria
	for:
		add $a0, $zero, $s1 	# Passando r como argumento
		add $a1, $zero, $t0		# Passando i como argumento
		jal pow
		
		mul $s2, $t0, $v0 		# S2 = i * v0
		add $s3, $s3, $s2
		add $a0, $zero, $s2   # Passando s2 como argumento
		jal print
		
		addi $t0, $t0, 1
		bne $t0, $s0, for
		
		add $a0, $zero, $s3   # Imprime resposta final
		jal print
	j exit
	
	
	
print:
	addi $v0, $zero, 1
	syscall
	addi $a0, $0, 0xA #ascii code for LF, if you have any trouble try 0xD for CR.
  addi $v0, $0, 0xB 
  syscall
	jr $ra
	
	
pow:
	addi $v0, $zero, 1    		# Seta retorno
	slti $t2, $a1, 1					# Conmpara  a1 < 1, se sim seta em t2 1, se a1 for maior seta 0
	beq $t2, $zero, continue  # Pula para o label continue se a1 é maior que 1
	jr $ra                    # Se a1 é menor que 1, zero no nosso caso, retorne 1, porque todo numero elevado a 0 é 1

	continue:
		mul $v0, $v0, $a0
		addi $a1, $a1, -1
		bne $a1, $zero, continue
		jr $ra
		
				
exit:
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	