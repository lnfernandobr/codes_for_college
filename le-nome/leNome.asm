;compilar:   nasm -g -f elfx hello.asm  onde x=32 ou x=64, geraria um hello.o
;linkeditar: ld hello.o  -o hello      o objeto será o hello determinado pelo -o 
;executar:   ./hello

; ISTO É UM COMENTARIO

section .data                        ;definindo a seção ou segmento de dados (variáveis)
  TECLADO equ 0
  TELA    equ 1
  msgNome db 'Entre com seu nome: '  ;definindo uma string 
  tamMsgNome   equ $ - msgNome        ;constante com o tamanho da string 
  strNome times 20 db 0               ;char strNome[20]={0,0,0,0,...0}
  msgBoasVindas db 'Bem Vindo, '
  tamMsgBoasVindas equ $ - msgBoasVindas
  numCharLidos db 0 
;enddata

%macro _printf 2
   ;salva os valores corrente dos registradores
  push rax
  push rbx         
  push rcx
  push rdx

  mov eax,4             ;escrita
  mov ebx,TELA             ;fd=1, na tela
  mov ecx,%1            ;buffer a ser gravado
  mov edx,%2            ;tamanho a ser gravado 
  int 0x80

  ;recupera os valores salvos dos registradores
  pop rdx
  pop rcx
  pop rbx                     
  pop rax
%endmacro

section .text                  ;segmento ou seção de codigo
  global _start                ;diretiva que define o inicio de codificacao (label _start)
  
  _start:                      ;label 
  
  ;mov rax,4                    ;seta serviço 4, de impressão  
  ;mov rbx,1                    ;seta o local de impressão, a tela do computador
  ;mov rcx,msgNome              ;indica o que irá imprimir,
  ;mov rdx,tamMsgNome           ;indica o numero de caracteres a imprimir 
  ;int 0x80                     ;realiza a interrupção (syscall), conforme o serviço solicitado

  _printf msgNome, tamMsgNome

  ;         ax      bx        cx           dx
  ;ssize_t read(int fd, void *buf, size_t count);
  ;    numCharLidos=read(     0,      strNome,   20);
  ;    if (numCharLidos==-1) printf("Erro de leitura"); 
  ;    numCharLidos é a quantidade de caracteres digitados 
  mov rax,3     ;serviço de leitura, read 
  mov rbx,TECLADO
  mov rcx,strNome
  mov rdx,20
  int 0x80        ; retorna o número de caracteres digitados em rax
  ;dec rax        ; o nasm guarda os caracteres lidos incluindo o <ENTER>
  mov [numCharLidos], byte rax     ;numCharLidos=rax
  
  ;ssize_t write(int fd, void *buf, size_t count);
  mov rax,4                    ;seta serviço 4, de impressão  
  mov rbx,TELA                 ;seta o local de impressão, a tela do computador
  mov rcx,msgBoasVindas              ;indica o que irá imprimir,
  mov rdx,tamMsgBoasVindas           ;indica o numero de caracteres a imprimir 
  int 0x80                     ;realiza a interrupção (syscall), conforme o serviço solicitado
  
  mov rax,4                    ;seta serviço 4, de impressão
  mov rbx, TELA                ;seta o local de impressão, a tela do computador
  mov rcx, strNome             ;imprime o nome
  mov rdx,[numCharLidos]      ;o numero de caracteres a imprimir
  int 0x80

  mov rax,1                    ;seta serviço 1, função exit
  mov rbx,0                    ;passa o parâmetro 0 (exit(0) em C)
  int 0x80                     ;realiza a interrupção
;endtext

