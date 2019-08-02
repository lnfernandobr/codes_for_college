section .data
  msg1 db "Fernando esse é o primeiro teste com modulos!", 0Ah, 0h
  msg2 db "Fernando esse é o Segunda  teste com coração! -q", 0Ah, 0h
  ; 0h denota um byte nulo e um byte nulo depois que uma string informa a montagem onde termina na memória

%include 'functions.asm'

section .text
  global _start
    _start:
      mov rax, msg1
      call printf

      mov rax, msg2
      call printf
      call exit
