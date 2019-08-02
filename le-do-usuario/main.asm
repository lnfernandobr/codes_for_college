; section .bss
;   variableName1 RESB 1 ; Reserva espaço para 1 byte
;   variableName2 RESW 1 ; Reserva espaço para 1 word
;   variableName3 RESD 1 ; Reserva espaço para 1 double word
;   variableName4 RESQ 1 ; Reserva espaço para 1 double float de precisao
;   variableName4 REST 1 ; Reserva espaço para 1 extended precision float

%include "functions.asm"

section .data
  msg1 db "Por favor, entre com seu nome: ", 0, 10
  msg2 db "Oi ", 0h

section .bss
  name resb 255 ; Reserva 255 bytes na memoria para a entrada da string

section .text
global _start
  _start:

    mov rax, msg1   ; Movendo msg para registrador para imprimir
    call printf     ; Chamando funçãotf

    mov rdx, 255    ; Movendo numero de bytes para ler, no registraor rdx
    mov rcx, name   ; Espaço reservado para guardar a entrada, (buffer)
    mov rbx, 0      ; Gravando no arquivo stdin (teclado)
    mov rax, 3      ; Invocando sys_read()
    int 80h         ; Chamando kernel para realizar operações

    mov rax, msg2   ; Movendo msg para registrador para imprimir
    call printf     ; Chamando função

    mov rax, name   ; Movendo msg para registrador para imprimir
    call printf     ; Chamando função

    call exit       ; Saindo do programa
