; foo(rdi, rsi, rdx, rcx, r8, r9)
extern printf
extern exit

section .data
    celsius times 4 dw 40,20,30, 90
    msgResultado db "Resultado: ", 10, 0
    formatoInteiro db "%d", 10, 0
    formatoString  db "%s", 10, 0


section .text
    global main    ;quando se usa gcc para linkar usa-se main

  main:
    mov rcx, 4
    xor rdx, rdx
    lea rbx, [celsius]

  l1:
    xor rdx, rdx
    push rcx
    push rdx
    mov ax, [rbx+rdx]
    pop rdx
    add rdx, 2
    add ax, 273
    mov rdi, formatoInteiro
    mov rsi, rax
    call printf
    pop rcx
    dec rcx
    jnz l1

    mov rax,1             ;system call number (sys_exit)
    int 80h              ;call kernel
