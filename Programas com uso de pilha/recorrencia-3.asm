# T(n) =  (2T(n/2) * 2T(n/3)) + 2T(n/4) + n 							P/ n > 1
# T(n) = 1 																								P/ n <= 1

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
	add $s2, $zero, 2
	add $s3, $zero, 3
	add $s4, $zero, 4
	
	add $sp, $sp, -16
	sw $ra, 0($sp)
	sw $a0, 4($sp)
	
	slti $t0, $a0, 2
	beq $t0, $zero, continue
	addi $sp, $sp, 16
	add $v0, $zero, 1
	jr $ra
	
	continue:
		div $a0, $s2
		mflo $a0
		jal T
		
		mult $s2, $v0
		mflo $v0
		sw $v0, 8($sp)
		
		lw $a0 4($sp)
		div $a0, $s3
		mflo $a0	
		jal T	
		
		mult $s2, $v0
		mflo $v0
		sw $v0, 12($sp)

		lw $a0 4($sp)
		div $a0, $s4
		mflo $a0
		jal T
		
		lw $ra 0($sp)
		lw $a0 4($sp)
		lw $t0 8($sp)
		lw $t1 12($sp)
		
		mult $s2, $v0
		mflo $v0
	
		mult $t1, $t0
		mflo $t4
		
		add $v0, $v0, $t4
		add $v0, $v0, $a0
		
		addi $sp, $sp, 16
		jr $ra
	

fim: