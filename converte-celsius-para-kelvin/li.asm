;===========================================================================
;
;             _Str2Num num1,strNum1,tamNum1;
;                       %1   %2      %3
;===========================================================================


%macro _Str2Num 3

  push rax
  push rbx
  push rcx

  xor eax, eax     ;zera eax
  xor rcx,rcx
  mov ebx, %2  ; lea ebx,[strNum1]
  ;eu quero um byte, se fizer mov cx, pego dois bytes a partir de &strNum
  ;se fizer mov ecx, pego 4 bytes, mov rax, pego 8 bytes
  mov cl,[%3]     ;ecx = *tamanhoStr
  ;Uma maneira de pegar somente o byte de um endereço seria
  ;and ecx,0x00ff
  xor esi,esi;zera o índice
%% gera_num1:
  mov dl,10       ;dl é 1 byte, dx = dh:dl, 16 bits, edx tem 32 bits, rdx 64 bits
  mul dl          ;não é permitido operação mul 10
                  ;tem que ser variavel ou registrador, o resultado ficará em ax
  sub byte [ebx+esi],'0'
  add al, [ebx+esi]
  inc esi
  dec cl
  cmp cl,0
  jnz gera_num1
  mov [Num1],ax ;passa o valor
  pop  rax
  pop  rbx
  pop  rcx
