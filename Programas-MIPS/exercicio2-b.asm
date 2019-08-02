# s1 = a1
# s2  = q 
# s3 = n

main:
#leia a1
addi $v0, $zero, 5 # Lê valor e coloca no registrador V0
syscall 
add $s1, $0, $v0

#leia q
addi $v0, $0, 5
syscall 
add $s2, $0,$v0


#leia n
addi $v0, $0, 5
syscall 
add $s3, $0, $v0

add $t0, $0, 1 # contador

# formula: a1 * q^(n-1);


for:
	add $a0, $zero, $s2
	add $a1, $zero, $s3
	jal pow
	
	mul $a0, $v0, $s1
	jal print
	
	addi $s3, $s3, -1
bne $s3, $zero, for

addi $a0, $0, 1
jal print

j end

print:
	add $v0, $0, 1
	syscall
	addi $a0, $0, 0xA #ascii code for LF, if you have any trouble try 0xD for CR.
  addi $v0, $0, 0xB 
  syscall
	jr $ra

pow:
	addi $v0, $zero, 1
	enq:
		mul $v0, $v0, $a0
		addi $a1, $a1, -1
	bne $a1, $zero, enq
	jr $ra
	
end:




