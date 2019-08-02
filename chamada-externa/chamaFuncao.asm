;compilar e linkar:
;nasm chamaFuncao.asm -f elf64
;gcc chamaFuncao.o -o chamaFuncao, linka com gcc

extern printf

section .data
  msg db "Mensagem para aparecer!", 0
  formato db "%s", 10, 0

section .text
  global main

    main:
      push rbp ; Guarda rbp na pilha para garantir que nenhum erro aconteça.

      ; Passa os parametros e chama as funções no C
      mov rdi, formato  ; 1 Parametro
      mov rsi, msg      ; 2 Parametro



      ; mov rax, 0        ;      PRECISA ?

      call printf       ; Chama função printf do C
      pop rbp           ; Pega rbp da pilha
      mov rax, 1        ; exit code 0

      ; ret               ; Retorna           PRECISA ?
