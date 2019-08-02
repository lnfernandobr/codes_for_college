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
    lea rbx, [celsius]
    xor rcx, rcx

  looping:

    xor rax, rax
    mov ax, [rbx + rcx]
    add ax, 273

    push rax
    push rbx
    push rcx

    mov rdi, formatoInteiro
    mov rsi, rax
    call printf

    pop rcx
    pop rbx
    pop rax

    add rcx, 2
    cmp rcx, 8
    jl looping
    call exit
