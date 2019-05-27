	#leia a ($s0)
addi $v0,$zero,5
syscall
add $s0,$zero,$v0

	#leia b ($s1)
addi $v0,$zero,5
syscall
add $s1,$zero,$v0

	#if(a == b)//melhor forma
bne $s0,$s1, nprint1
		#print 1
addi $v0,$zero,1
addi $a0,$zero,1
syscall
nprint1:

	#if(a == b) //outra forma
beq $s0,$s1, print1
j othernprint1
print1:
		#print 1		
addi $v0,$zero,1
addi $a0,$zero,1
syscall
othernprint1:	
		
	#if(a!= b)
beq $s0,$s1,nprint2
		#print 2
addi $v0,$zero,1
addi $a0,$zero,2
syscall
nprint2:		
		
	#if(a < b)
slt $t0,$s0,$s1
beq $t0,$zero,nprint3
		#print 3
addi $v0,$zero,1
addi $a0,$zero,3
syscall		
nprint3:		
		
	#if(a > b)
slt $t0,$s1,$s0
beq $t0,$zero,nprint4
		#print 4
addi $v0,$zero,1
addi $a0,$zero,4
syscall		
nprint4:

	#if(a <= b) ->> if(!(a>b))
slt $t0,$s1,$s0
bne $t0,$zero,nprint5
		#print 5
addi $v0,$zero,1
addi $a0,$zero,5
syscall		
nprint5:		
		
	#if(a >= b)->> if(!(a<b))
slt $t0,$s0,$s1
bne $t0,$zero,nprint6
		#print 6
addi $v0,$zero,1
addi $a0,$zero,6
syscall		
nprint6:
