
main:
	addi $a0,$zero,9
	jal fib
	add $a0,$zero,$v0
	addi $v0,$zero,1
	syscall
	j exit

fib:
	addi $sp, $sp, -12
	sw $ra, 4($sp)
	sw $a0, 0($sp)
	slti $t0, $a0, 3
	beq $t0, $zero, else
	addi $sp, $sp, 12 	
	addi $v0, $zero, 1 #for n<=2
	jr $ra

else: 					#for n>2
	addi $a0,$a0,-1
	jal fib  				#fib(n-1)
	
	sw $v0, 8($sp)
	addi $a0, $a0, -1
	jal fib         #fib(n-2)
	
	lw $t1,8($sp)
	add $v0,$v0,$t1
	lw $ra,4($sp)
	lw $a0,0($sp)
	addi $sp,$sp,12
	jr $ra

exit:



