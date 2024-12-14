.586P
.model flat, stdcall
includelib libucrt.lib
includelib kernel32.lib
includelib ../Debug/LIB.lib
ExitProcess PROTO : DWORD

SetConsoleCP PROTO : DWORD

SetConsoleOutputCP PROTO : DWORD

Pow PROTO : BYTE, : BYTE 

Sum PROTO : BYTE, : BYTE 

writestr PROTO : DWORD 

writeint PROTO : BYTE 

writebool PROTO : BYTE 

writechar PROTO : BYTE 

.stack 4096

.const
L0 SBYTE 1 ; byte
L1 SBYTE 4 ; byte
C3 BYTE 116 ; symbol
L3 SBYTE 3 ; byte
L5 DB "OK", 0 ; text

.data
result SDWORD 0 ; int
b SDWORD 0 ; int
d SDWORD 0 ; int
my_c SDWORD 0 ; int
re BYTE 0 ; ch

.code

FgetMul PROC uses ebx ecx edi esi, num1, num2
; string #2 : i=iiv
mov eax, num1
mov result, eax
; Addition
mov eax, num1
add eax, num2
mov result, eax

; return
mov eax, result
ret
FgetMul ENDP

main PROC
Invoke SetConsoleCP, 1251
Invoke SetConsoleOutputCP, 1251

; string #8 : i=l
movzx eax, L0
mov b, eax

; string #9 : i=l
movzx eax, L1
mov d, eax

; string #10 : i=ii@2##
mov eax, d
push eax
mov eax, b
push eax
CALL FgetMul
mov my_c, eax

mov eax, my_c
push eax
CALL writeint

; string #13 : i=l
mov al, C3
mov re, al 
push eax
movzx eax, re
push eax
CALL writechar
pop eax


If_Begin1: 
mov eax, d
movzx ebx, L3
cmp eax, ebx
jg If_End1
jmp If_End2
If_End1:

push offset L5
CALL writestr
If_End2:
push -1
call ExitProcess
main ENDP
end main
