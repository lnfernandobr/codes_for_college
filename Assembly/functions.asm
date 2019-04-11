;----------------------------- MACRO PRINTF -----------------------------
  ; macro nomeDaMacro numeroDeParametros: macro _printf 2
  ; FORMATO = _printf MsgNum1, tmsgNum1

  %macro _printf 2
    ;Salva Valores dos registradores na stack
    push rax
    push rbx
    push rcx
    push rdx

    mov rax, 4             ; Avisa que vai escrever
    mov rbx, TELA          ; Saida dos dados, TELA
    mov rdx, %2            ; Move o tamanho da mensagem passado no Parametro %2 para o Reg. RDX
    mov rcx, %1            ; Move o endereço do Parametro %1 para o Reg RCX para poder imprimir
    int 0x80               ; Chama Kernel para executar operação

    ; Recupera os valores salvos dos registradores
    pop rdx
    pop rcx
    pop rbx
    pop rax
  %endmacro



;----------------------------- MACRO SCANF -----------------------------
  ; macro nomeDaMacro numeroDeParametros: macro _scanf 2
  ; FORMATO = _scanf strNum1, 10
  %macro _scanf 2

    ; Salva valores do registradores na stack
    push rbx
    push rcx
    push rdx

    mov rax, READ             ; Avisa que será Serviço de leitura, read.
    mov rbx, TECLADO          ; Lendo stdion - teclado
    mov rcx, %1               ; Lendo primeiro Parametro passado a função
    mov rdx, %2               ; Movendo segundo Parametro para RDX
    int 0x80                  ; Chama Kernel para executar operação

    ; Recupera valores do registradores na stack
    pop rdx
    pop rcx
    pop rbx
  %endmacro



;----------------------------- MACRO _Str2Num -----------------------------
  ; macro nomeDaMacro numeroDeParametros: macro _Str2Num 3
  ; FORMATO =_Str2Num Num1, strNum1, tamNum1
  %macro _Str2Num 3

    ; Salva Valores na Pilha
    push rax
    push rbx
    push rcx

    xor eax, eax      ; Zera o registrador EAX
    xor rcx, rcx      ; Zera o registrador RCX

    mov ebx, %2    ; *******lea ebx,[strNum1]

    ;*******eu quero um byte, se fizer mov cx, pego dois bytes a partir de &strNum
    ;*******se fizer mov ecx, pego 4 bytes, mov rax, pego 8 bytes

    mov cl, [%3]     ; ECX = *tamanhoStr

    ; ******* Uma maneira de pegar somente o byte de um endereço seria
    ;******* and ecx,0x00ff

    xor esi, esi   ;Zera o índice

      ; Label
      %%gera_num1:
        mov dl, 10      ;dl é 1 byte, dx = dh : dl, 16 bits, edx tem 32 bits, rdx 64 bits
        mul dl          ;não é permitido operação mul 10
                        ;tem que ser variavel ou registrador, o resultado ficará em ax

        sub byte [ebx + esi], '0'
        add al,  [ebx + esi]
        inc esi                   ; Incrementa esi
        dec cl                    ; Decrementa cl
        cmp cl, 0                 ; Compara se cl == 0
      jnz %%gera_num1           ; se a comparação da linha acima for verdadeira sai do loop, caso contrario volta ao label.

    mov [%1], ax  ;Mova o valor de ax para o Parametro

    ; Recupera Dados da pilha
    pop rcx
    pop rbx
    pop rax
  %endmacro


;----------------------------- IMPRIME Mensagem Resultado -----------------------------
printfExtern:
   push rcx
   mov eax, 4             ;escrita
   mov ebx, 1             ;fd=1, na tela
   mov ecx, MsgResultado   ;&buffer a ser gravado
   mov edx, tMsgResultado ;tamanho a ser gravado
   pop rcx
ret
;----------------------------- *************************** -----------------------------


_fimprimeTopo:
  pop rax
  mov byte [aux],ah ; pega os restos

  ; Imprime o caractere
  mov eax,4      ;escrita
  mov ebx,1      ;fd=1, na tela
  push rcx       ;guarda valor antigo do contador
  mov ecx,aux    ;&buffer a ser impresso
  mov edx,1      ;tamanho a ser gravado
  int 0x80
  pop rcx
  dec cl
  cmp cl,0
  jnz _imprimeTopo
ret


pulLinha:
  mov eax,4          ;escrita
  mov ebx,1          ;fd=1, na tela
  mov ecx, pulaLinha  ;&buffer a ser impresso
  mov edx,1          ;tamanho a ser gravado
  int 0x80

; Exit
exit:
  mov eax,1
  mov ebx,0
  int 0x80
