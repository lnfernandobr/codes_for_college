# s1 = n
# s2 =  a1
# s3 = r
main:
#leia n
addi $v0, $zero, 5 # Lê valor e coloca no registrador V0
syscall 
add $s1, $0, $v0

#leia a1
addi $v0, $0, 5
syscall 
add $s2, $0,$v0


#leia r
addi $v0, $0, 5
syscall 
add $s3, $0, $v0

add $t0, $0, 1 # contador

# formula: a1 + (n-1) * r;
for:
	addi $t0, $s1, -1
	mul $t4, $t0, $s3
	add $a0, $s2, $t4
	jal print
	addi $s1, $s1, -1
bne $s1, $zero, for

j end

print:
	add $v0, $0, 1
	syscall
	addi $a0, $0, 0xA #ascii code for LF, if you have any trouble try 0xD for CR.
  addi $v0, $0, 0xB 
  syscall
	jr $ra

end:




