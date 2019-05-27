#leia dois valores a e b e imprima a sequência 
#crescente de a até b

#leia a>0 (a => $s0)
addi $v0,$zero,5
syscall
add $s0,$zero,$v0
#leia b>0 (b => $s1)
addi $v0,$zero,5
syscall
add $s1,$zero,$v0
#enquanto a<=b -> !(b<a)
enquanto:
	slt $t0,$s1,$s0
	bne $t0,$zero,fim
	addi $v0,$zero,1 #impressao
	add $a0,$zero,$s0#carrega em $a0 o valor a ser impresso
	syscall
	addi $s0,$s0,1#a++
	j enquanto
fim:
