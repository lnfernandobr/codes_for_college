;compilar:   nasm -g -f elfx hello.asm  onde x=32 ou x=64, geraria um hello.o
;linkeditar: ld hello.o  -o hello      o objeto será o hello determinado pelo -o 
;executar:   ./hello

; ISTO É UM COMENTARIO

section .data                     ;definindo a seção ou segmento de dados (variáveis)
  hello db 'Hello World!',10,13  ;definindo uma string "Hello\n"
  tam   equ $ - hello            ;constante com o tamanho da string hello
;enddata

section .text                  ;segmento ou seção de codigo
  global _start                ;diretiva que define o inicio de codificacao (label _start)
  _start:                      ;label 
  
  mov eax,4                    ;seta serviço 4, de impressão  
  mov ebx,1                    ;seta o local de impressão, a tela do computador
  mov ecx,hello                ;indica o que irá imprimir, o conteúdo de hello
  mov edx,tam                  ;indica o numero de caracteres a imprimir a partir do inicio de hello
  int 0x80                     ;realiza a interrupção (syscall), conforme o serviço solicitado
  mov eax,1                    ;seta serviço 1, função exit
  mov ebx,0                    ;passa o parâmetro 0 (exit(0) em C)
  int 0x80                     ;realiza a interrupção
;endtext

