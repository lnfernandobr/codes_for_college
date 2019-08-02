;--------------------- COMPILAR -----------------------------
; Gerar '.o' ===  nasm -g -f elfX name.asm  X = 64 ou X = 32
; Linka '.o' ===  ld name.o -o name
;--------------------- COMPILAR -----------------------------

%include 'functions.asm'
section .data

  ; Denifição de variaveis
  MsgNum1          db 'Entre com o primeiro número: ', 0Ah, 0h
  MsgNum2          db 'Entre com o segundo número: ',  0Ah, 0h
  MsgResultado     db 'Resultado da soma: ', 0Ah, 0h
  tMsgResultado   equ $- MsgResultado
  tamNum1          db 0
  tamNum2          db 0
  tmsgNum1        equ $- MsgNum1 ; $ É a posição após a alocação de bytes por MsgNum1
  tMsgNum2        equ $- MsgNum2
  strNum1 times 10 db 0 ; Mesmo que char strNum1[] = { 0, 0, 0, ..., 0 }, um vetor com 10 celulas e zerado
  strNum2 times 10 db 0
  Num1             db 0
  Num2             db 0
  aux              db 0
  pulaLinha        db 10
  TECLADO equ 0
  TELA equ 1
  READ equ 3


section .text
  global _start

_start:
  _printf MsgNum1, tmsgNum1
  _scanf strNum1, 10

  mov [tamNum1], al     ; Obtem o tamanho da string lida.
  dec byte [tamNum1]    ; guarda o <ENTER> junto

  _printf MsgNum2, tMsgNum2
  _scanf strNum2, 10

  mov [tamNum2], al          ; Pega o tamanho da string lida
  dec byte [tamNum2]

  _Str2Num Num1, strNum1, tamNum1
  _Str2Num Num2,strNum2, tamNum2

  mov al, [Num1]
  add al, [Num2]    ; Resultado da soma em *Num2

  ; Zera os registradores
  xor rcx,rcx
  xor rdx,rdx
  xor rbx,rbx

  _divide:
    mov rbx, 10
    div bl     ;nao se permite div 10, div 3, div 20, etc

    ; AL Fica o quociente da divisao, AH fica o resto
    add ah, '0'; Somando o resto AH da divisao com '0'

    push rax      ;Deveriamos empilhar o AH, mas temos também que empihlar todo o RAX

    and ax, 0xff  ; Zeramos AH, ficando somente com a parte AL (AX tem 2 bytes e dois nibble de 1 byte)
    inc cl        ; Conta elementos na pilha, faz o Incremento
    cmp al, 0     ; Compara AL para saber se omesmo é 0, se sim sai do looping
    jnz _divide   ; Se AL Não for zero, volta pro label _divide


  call printfExtern

_imprimeTopo:
  pop rax
  mov byte [aux], ah ; Pega os restos

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

call pulLinha
call exit
