#leia um valor n>0 e imprima a soma de 1 até n

#leia n e mapeie para $s0
addi $v0,$zero,5
syscall
add $s0,$zero,$v0

addi $t0,$zero,0#zera o registrador $t0 p/ utiliza-lo
								# como acumulador da soma
#do{
do:
add $t0,$t0,$s0 # $t0+=n
addi $s0,$s0,-1  # n--
bne $s0,$zero,do
#}while(n>0)

addi $v0,$zero,1
add $a0,$zero,$t0
syscall