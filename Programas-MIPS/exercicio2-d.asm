#Impressão dos n primeiros termos da série ai = i * (r)^i

main:
	addi $v0, $0, 5 # Seta serviço de leitura
	syscall
	add $s0, $0, $v0 # guarda em s0 o valor lido
	
	addi $v0, $0, 5 # Seta serviço de leitura
	syscall	
	add $s1, $0, $a0 # guarda em s1 o valor lido
	
	
	addi $t0, $0, 0
	addi $s2, $0, 0
	
	for:
		add $a0, $0, $s1
		add $a1, $0, $t0
		jal pow
		add $a0, $zero, $v0
		jal print
		addi $t0,$t0, 1
	bne $t0, $s0, for
	j fim
print:
	add $v0, $0, 1
	syscall
	addi $a0, $0, 0xA #ascii code for LF, if you have any trouble try 0xD for CR.
  addi $v0, $0, 0xB 
  syscall
	jr $ra
	
	
	pow:
		addi $v0, $0, 1
		slti $t2, $a1, 1
		beq $t2, $0, nFor
		jr $ra
		#a0 = r
		#a1 = n
		nFor:
			mul $v0, $v0, $a0
			addi $a1, $a1, -1
		bne $a1, $0, nFor
	
		jr $ra
	
	fim:
	
	
	
