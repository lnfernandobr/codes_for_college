;compilar:   nasm -g -f elfx lesenha.asm  onde x=32 ou x=64, geraria um lesenha.o
;linkeditar: ld lesenha.o  -o lesenha      o objeto será o lesenha determinado pelo -o 
;executar:   ./lesenha

section .data
  ;=========================================================================================================
  ;Na plataforma windows, o pulo de linha deveria ser a junção do codigo 10 (0xA) e o codigo 13 (0xD)
  ;No linux, só precisa do 10

  msgLendo   db 'Entre com a senha: '   ;definição de uma string
  TAM_MSG_LENDO equ $-msgLendo
  msgCongrat db 'Parabens, esta se tornando um Hacker',10,13
  TAM_MSG_CONG equ $-msgCongrat
  msgErro db 'Senha inconsistente',10,13
  TAM_MSG_ERRO equ $-msgErro
  TAM_MAX  equ 10
  strLida times 10 db 0

;enddata
section .text

  global _start

  _start:
  
    ;syscall write(4), retorna quantidade gravada em eax
    mov eax,4             ; escrita
    mov ebx,1             ; fd=1, na tela
    mov ecx,msgLendo      ;buffer a ser gravado
    mov edx,TAM_MSG_LENDO ;tamanho a ser gravado , _printf(msgLendo,TAM_MSG_LENDO)
    int 0x80
  
    mov eax,3             ; leitura
    mov ebx,0             ;fd=0, teclado
    mov ecx,strLida       
    mov edx,TAM_MAX       ;tamanho maximo de leitura
    int 0x80              ;faz a chamada de leitura, guarda em eax O tamanhoDigitado + 1 (devido ao <ENTER>)
    cmp eax,0             ;guarda o tamanho lido no acumulador ax
    jz  falha
    xor esi,esi           ;esi=0, mais econômico do que mov esi,0
    ;esi será o índice para percorrer a strLida
  
    cmp byte [strLida+esi],33h  ; compara com '3', poderia também fazer cmp byte [strlida+esi],'3'
    ; Em C seria strcmp(strLida[esi],'3')
    jne falha                   ; se não for igual a '3' salta para mensagem de erro
    inc esi
    cmp byte [strLida+esi],39h  ; compara com '9'
    jne falha
    inc esi
    cmp byte [strLida+esi],37h  ; compara com '7'
    jne falha
    inc esi
    cmp byte [strLida+esi],31h  ; compara com '1'
    jne falha 
    
    mov eax,4                   ; da a mensagem de parabéns na tela
    mov ebx,1
    mov ecx,msgCongrat
    mov edx,TAM_MSG_CONG
    int 0x80
    jmp fim                     ;salta incondicionalmente para fim

  falha:
    mov eax,4
    mov ebx,1
    mov ecx,msgErro
    mov edx,TAM_MSG_ERRO
    int 0x80

  fim:
    mov eax,1                  ; exit(0)
    mov ebx,0
    int 0x80

  
  
