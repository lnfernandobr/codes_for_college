; Compilar: nasm helloWorldlen.asm -f elf64
; Linkeditar: ld helloWorldlen.o -o programa
; Executar: ./programa

section .data
  msg db "Wello World!", 0Ah  ; Define a string msg e adiciona '\0' ao final com o (0Ah)

section .text
  global _start       ; Define Por onde o programa vai iniciar, no label _start

    _start:
      mov rax, msg    ; Movendo o endereço da variavel msg para o registrador rax de 64bits
      call srtlen     ; Chamando nossa função (strlen) para calcular tamanho da String!

      mov rdx, rax    ; A Função deixa o resultado no rax e depois movemos para rdx.
      mov rcx, msg    ; Movendo endereço da string msg para registrado rcx

      mov rbx, 1      ; Chamando para escrever na tela STDOUT
      mov rax, 4      ; Chamando sys_write e escrevendo na tela (10)
      int 80h         ; Chamando kernel para executar

      mov rbx, 0      ; Retorna 0 pro rbx para chamada exit
      mov rax, 1      ; Chaman sys_exit e sai do programa
      int 80h         ; Chamando kernel para executar

    srtlen:         ; Declaração da função
      push rbx      ; Armazenamos na Pilha o valor de rbx para preseva-lo enquanto usamos ele na função
      mov rbx, rax  ; Move o endereço de rax para rbx, ambos apontam para mesmo segmento de memoria

    nextchar:
      cmp byte [rax], 0 ; Compara se o byte que está apontado pelo registrador é zero, se for jz pra finished
      jz finished       ; Se rax aponta para o 0, pula pro label finished
      inc rax           ; Incrementa em 1 byte o rax
      jmp nextchar      ; Looping, voltando para label nextchar


    finished:
      sub rax, rbx      ; Subtraia o endereço de rbx - rax
      pop rbx           ; Coloca o valor da pilha de volta no rbx
      ret               ; Retornar para onde a função foi chamada
