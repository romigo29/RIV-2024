.586P
.model flat, stdcall
includelib libucrt.lib
includelib kernel32.lib
includelib ../Debug/LIB.lib
ExitProcess PROTO : DWORD

SetConsoleCP PROTO : DWORD

SetConsoleOutputCP PROTO : DWORD

module PROTO : BYTE 

rest PROTO : BYTE, : BYTE 

writestr PROTO : DWORD 

writeint PROTO : BYTE 

writebool PROTO : BYTE 

writechar PROTO : BYTE 

.stack 4096

.const
L0 SBYTE 10 ; byte
L1 SBYTE 3 ; byte
L2 SBYTE -2 ; byte

.data
r1 SDWORD 0 ; int
m1 SDWORD 0 ; int

.code
main PROC
Invoke SetConsoleCP, 1251
Invoke SetConsoleOutputCP, 1251

; string #3 : i=ll@2##
movsx eax, L1
push eax
movsx eax, L0
push eax
CALL rest
mov r1, al

mov eax, r1
push eax
CALL writeint

; string #6 : i=l@1#
movsx eax, L2
push eax
CALL module
mov m1, al

mov eax, m1
push eax
CALL writeint
push -1
call ExitProcess
main ENDP
end main
