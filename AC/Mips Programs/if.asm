# Leia a e b, faça condicional, se a for maior que b, imrpima A. Encerre o programa
# ---------------------------------------------------------------------------------
# Fernando Lima - 2019
# UEMS - Ciências da computação

# T0 = a
# T1 = B

addi $v0, $zero, 5 # Lê valor e coloca no registrador V0
syscall 

add $t0, $zero, $v0 # Move do registrador V0 para T0 o valor LIDO

addi $v0, $zero, 5
syscall 

add $t1, $zero, $v0 # Move do registrador V0 para T1 o valor LIDO

# if(a > b) {

# }

slt $t2, $t0, $t1    # Se T0 for menor que T1 então T2 recebe 1, se T0 for maior que T1, T2 recebe 0
beq $t2, 1, continue # Se T2 for igual a 1, pule para o label continue e não faça a impressao

addi $v0, $zero, 1   # Seta serviço de impressao stdout
add $a0, $zero, $t0  # Coloca valor em $a0
syscall              # Chama o sistema

continue:	
