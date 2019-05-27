main:
	addi $a0, $zero, 5
	jal fact
	addi $v0, $zero, 1
	addi $a0, $v0, 0
	syscall
	
fact: 
	addi $sp, $sp, -8
	sw, $ra, 4($sp)
	sw, $a0, 0($sp)
	slti $t0, $a0, 1
	beq $t0, $zero, l1
	addi $v0, $zero, 1
	addi $sp, $sp, 8
	jr $ra
	
	l1: 
		addi $a0, $a0, -1
		jal fact
		lw $ra, 4($sp)
		lw $a0, 0($sp)
		addi $sp, $sp, 8
		mul $v0, $a0, $v0
		jr $ra
