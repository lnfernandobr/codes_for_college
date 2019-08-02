%include 'functions.asm'

section .data
msg db "qufqfewqfewqfewqfe", 0
section .text
global _start
  _start:
    mov rcx, 0      ; Rcx é inicializado com 0
    ; mov rax, msg
    ; call printf
    ; call exit
    ;
    nextNumber:
      inc     rcx             ; increment ecx

      mov     rax, rcx        ; move the address of our integer into eax
      add     rax, 48         ; add 48 to our number to convert from integer to ascii for printing
      push    rax             ; push eax to the stack
      mov     rax, rsp        ; get the address of the character on the stack

      call    printf          ; call our print function

      pop     rax             ; clean up the stack so we don't have unneeded bytes taking up space
      cmp     rcx, 10         ; have we reached 10 yet? compare our counter with decimal 10
      jne     nextNumber      ; jump if not equal and keep counting

      call    exit
