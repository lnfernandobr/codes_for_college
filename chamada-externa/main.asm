extern add
extern printf
extern exit

section .data
  format db "%d", 10, 0 ; 10 == '\n'  e    0 == '\0'


section .text
  global main

    main:
      push rbp            ; Guarda rbp na pilha para que não aconteça erros
      mov rdi, 6          ; Move 6 pro registrador rdi
      mov rsi, 2          ; Move 2 pro registrador rsi
      call add            ; Chama função add externa do c, add(2,6)
      mov  rdi, format    ; No parametro para printf em rdi passamos o formato
      mov rsi, rax        ; Passando o conteudo que será impresso, contido no registrador rax
      call printf         ; Chamando a função printf, rdi tem o formato, rsi contem o valor
      pop rbp             ; Recupera rbp da pilha
      xor rdi, rdi        ; Zera o registrador rdi
      call exit           ; Chama função exit do c

; nasm -f elf64 main.asm
; gcc -Wall -c add.c
; gcc main.o add.c -o mainCall
