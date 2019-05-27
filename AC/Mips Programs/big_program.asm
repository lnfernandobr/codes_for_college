# Crie um programa que leia 2 numeros, imprima o maio entre eles usando a estrutura if {} else {}... Logo apos a impressao desse 
# valor, faça um for indo de 0 até o maior valor difitado pelo usuario, faça a soma do intervalo e mostre na tela novamente.
# Para finalizar, pega todos os valores entre 0 e menor valor digitado pelo usuario e mostre os numeros pares
# ------------------------------------------------------------------------------------------------------------------------------
# Fernando Lima - 2019
# UEMS - Ciências da computação

# T0 = a
# T1 = B
# T2 = maiorValor
# T3 = sum (acumulador)
# T4 = menorValor 

main: 
	addi $v0, $zero, 5 # Lê valor e coloca no registrador V0
	syscall 

	add $t0, $zero, $v0 # Move do registrador V0 para T0 o valor LIDO

	addi $v0, $zero, 5
	syscall 

	add $t1, $zero, $v0 # Move do registrador V0 para T1 o valor LIDO


	# if(a > b) {
	# 	printf("%d", a);
	# } else {
	# 	printf("%d", b);
	# }

	slt $t2, $t0, $t1    # Se T0 for menor que T1 então T2 recebe 1, se T0 for maior que T1, T2 recebe 0
	beq $t2, 1, else # Se T2 for igual a 1, pule para o label continue e não faça a impressao
	
	addi $v0, $zero, 1     # Seta serviço de impressao stdout
	add $a0, $zero, $t0    # Color valor da variavel a em $a0
	syscall                # Chama o sistema
	add $t2, $zero, $t0    # Color na variavel T2=maiorValor o maior valor, 
	j continue             # Pule para o label continue para que não seja executado o label else.
	
	else: 
		addi $v0, $zero, 1   # Seta serviço de impressao stdout
		add $a0, $zero, $t1  # Coloca valor da operação realizada no laço do no registrador de impressao $s0
		syscall              # Chama o sistema
		add $t2, $zero, $t1  # Color na variavel T2=maiorValor o maior valor, 

	continue:
 		# int i 
  	# for(i = maiorValor; i > 0; --maiorValor) {
	  #		sum += maiorValor;		
	  # }
	  # printf("%d", sum);


	  add $s1, $zero, $t2
	  addi $t3, $zero, 0      # Zera o acumulador
  
  	for:
  		add $t3, $t3, $s1     # T3 += maiorValor 
	  	addi $s1, $s1, -1     # maiorVAlor--; = S1--;
	  bne $s1, $zero, for     # Pule para o label for enquanto o valor de s1 for diferente de zero
	  
  	addi $v0, $zero, 1     # Seta serviço de impressao stdout
		add $a0, $zero, $t3    # Color valor da variavel a em $a0
		syscall                # Chama o sistema
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  

