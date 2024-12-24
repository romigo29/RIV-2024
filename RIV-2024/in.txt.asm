.586P
.model flat, stdcall
includelib libucrt.lib
includelib kernel32.lib
includelib ../Debug/LIB.lib
ExitProcess PROTO : DWORD

SetConsoleCP PROTO : DWORD

SetConsoleOutputCP PROTO : DWORD

module PROTO : DWORD 

rest PROTO : DWORD, : BYTE 

writestr PROTO : DWORD 

writeint PROTO : BYTE 

writebool PROTO : BYTE 

writechar PROTO : BYTE 

.stack 4096

.const
L0 DWORD 3 ; int (4 bytes)
L1 DWORD 5 ; int (4 bytes)
L2 DWORD 4 ; int (4 bytes)
L3 DWORD 4 ; int (4 bytes)
L4 DWORD 5 ; int (4 bytes)
L5 DWORD 155 ; int (4 bytes)
L6 DWORD 198 ; int (4 bytes)
C8 BYTE 73 ; ch
L9 DB "dont mind", 0 ; str
L11 DB "Вывод строки", 0 ; str
L12 DWORD 0 ; int (4 bytes)
L13 DWORD 2 ; int (4 bytes)
L14 DWORD 1 ; int (4 bytes)

.data
yesor SDWORD 0 ; int
inversion SDWORD 0 ; int
yesand SDWORD 0 ; int
rv SDWORD 0 ; int
mv SDWORD 0 ; int
iv SDWORD 0 ; int
symbol BYTE 0 ; ch
stroka DWORD 0 ; str
flag BYTE 0 ; boolean

.code

FgetAnd PROC uses ebx ecx edi esi, a : SBYTE, b : SBYTE
; return
movsx eax, a
mov a, al
movzx eax, a
; Logical AND
mov al, a
and al, b
mov b, al
ret
FgetAnd ENDP


FgetMul PROC uses ebx ecx edi esi, a : DWORD, b : DWORD
; return
mov eax, a
; Multiplication
mov eax, a
imul eax, b
mov b, eax
ret
FgetMul ENDP

main PROC
Invoke SetConsoleCP, 1251
Invoke SetConsoleOutputCP, 1251

; string #11 : ;aboba
push L0
push L1
pop ebx
pop eax
or eax, ebx
push eax
mov yesor, eax

; string #12 : ;aboba
push L2
pop eax
not eax
push eax
mov inversion, eax

; string #13 : ;aboba
push L3
push L4
call FgetAnd
push eax 
mov yesand, eax

mov eax, yesor
push eax
CALL writeint
pop eax

mov eax, inversion
push eax
CALL writeint
pop eax

mov eax, yesand
push eax
CALL writeint
pop eax

; string #19 : ;aboba
push yesor
push yesand
CALL rest
push eax 
push eax
mov rv, eax

; string #20 : ;aboba
push inversion
CALL module
push eax 
push eax
mov mv, eax

mov eax, rv
push eax
CALL writeint
pop eax

mov eax, mv
push eax
CALL writeint
pop eax

; string #25 : ;aboba
push L5
push L6
call FgetMul
push eax 
mov iv, eax

mov eax, iv
push eax
CALL writeint
pop eax

; string #27 : ;aboba
movsx eax, C8
mov symbol, al

; string #28 : ;aboba
mov eax, OFFSET L9
mov [stroka], eax
push eax
movzx eax, symbol
push eax
CALL writechar
pop eax


push offset L11
CALL writestr

; string #32 : ;aboba
mov eax, L12
mov flag, al

If_Begin1: 
mov eax, inversion
mov ebx, L13
cmp eax, ebx
jg If_End1
jmp If_Else
If_End1:

; string #35 : ;aboba
mov eax, L14
mov flag, al
push eax
movzx eax, flag
push eax
CALL writebool
pop eax

jmp If_End2
If_Else:
push eax
movzx eax, flag
push eax
CALL writebool
pop eax

If_End2:

mov eax, yesor
push eax
CALL writeint
pop eax
push -1
call ExitProcess
main ENDP
end main
