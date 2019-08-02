; *****************************************************************************************************************
;                             Função para calular TAMANHO DA STRING                                               *
;******************************************************************************************************************

slen:
  push rbx      ; Armazena na pilha o valor de rbx para preserva-lo e enquanto usamos o rbx na função.
  mov rbx, rax  ; Move o endereço de rax para rbx, ambos apontam para o mesmo endereço da String

nextchar:
  cmp byte [rax], 0 ;Compara para ver se byte que  o registrador rax não está apontando é 0
  jz finished       ; Se rax aponta para 0, pula para o label finished
  inc rax;          ; Se não, Incrementa rax
  jmp nextchar      ; Looping até condição

finished:
  sub rax, rbx      ; Subtrai rbx de rax, assim rax fica com o comprimento da cadeia
  pop rbx           ; Volta valor de rbx da pilha para o registrador
  ret

;******************************************************************************************************************



; *****************************************************************************************************************
;                             Função para calular IMPRIMIR STRING                                                 *
;******************************************************************************************************************

printf:
  push rdx  ; - Quarda o valor de rdx na pilha
  push rcx  ; - Quarda o valor de rdx na pilha
  push rbx  ; - Quarda o valor de rdx na pilha
  push rax  ; <-
  call slen ; <- chamada


  mov rdx, rax      ; Move o tamanho da string que está no registrador rax para o registrado rdx
  pop rax           ; Recupera o que estava em rax, no caso o endereço da string que foi passada.

  mov rcx, rax      ; Move o endereço da msg que esta no registrador rax para o registrador rcx

  mov rbx, 1        ; Fala que vai escrever na tela
  mov rax, 4        ; Chama sys_write para escreve
  int 80h           ; Chama kernel


; Volta da pilha os valores de rbx, rcx e rdx, Logo em seguida retorna.
  pop rbx
  pop rcx
  pop rdx
  ret

;******************************************************************************************************************





; *****************************************************************************************************************
;                             Função para calular SAIR - EXIT                                                     *
;******************************************************************************************************************

exit:
  mov rbx, 0
  mov rax, 1
  int 80h
  ret
;******************************************************************************************************************







; *****************************************************************************************************************
;                             Função Imprimir stirng com avanço de linha                                          *
;******************************************************************************************************************

printfLF:
  call printf

  push rax        ; Colocando o registrador na pilha pra preseva-lo enquando usando na função
  mov rax, 0Ah    ; Colocan o caracter '\n' no registrador rax

  push rax        ; Coloca rax na pilha  ??????????????????????????????????????????????????????????????

  mov rax, rsp    ; Move o endereço do ponteiro da pilha atual para rax
  call printf     ;  Chama função printf
  pop rax         ; Remove caracter '\0' da pilha
  pop rax         ; Restaura valor de rax antes da função ser chamada
  ret

;******************************************************************************************************************
