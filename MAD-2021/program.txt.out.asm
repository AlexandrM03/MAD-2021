.586
.model flat, stdcall
includelib libucrt.lib
includelib kernel32.lib
includelib ../Debug/StaticLibrary.lib
ExitProcess PROTO :DWORD

EXTRN slen: proc
EXTRN scpy: proc
EXTRN mpow: proc
EXTRN mrand: proc
EXTRN BREAKL: proc
EXTRN OutputInt: proc
EXTRN OutputStr: proc
EXTRN OutputIntLn: proc
EXTRN OutputStrLn: proc

.stack 4096

.const
	L1 SDWORD 0
	L2 SDWORD 1
	L3 SDWORD 0
	L4 SDWORD 1
	L5 BYTE "Считаем до ", 0
	L6 BYTE ":", 0
	L7 BYTE " ", 0
	L8 SDWORD 5
	L9 BYTE " = 0? : ", 0
	L10 SDWORD 10
	L11 BYTE "Случайное число от 1 до 10: ", 0
	L12 BYTE "Александр", 0
	L13 BYTE "Артем", 0
	L14 BYTE ", длина = ", 0
	L15 BYTE "5^3 = ", 0
	L16 SDWORD 3
	L17 BYTE "6! = ", 0
	L18 SDWORD 6
	L19 BYTE "0xfff = ", 0
	L20 SDWORD 4095
	L21 BYTE "0x14de = ", 0
	L22 SDWORD 5342
	L23 BYTE "0x2ea = ", 0
	L24 SDWORD 746
	L25 SDWORD 4
	L26 SDWORD 14
	L27 SDWORD 7
	L28 SDWORD 16

.data
	buffer BYTE 256 dup(0)
	counti SDWORD 0
	facz SDWORD 0
	maini SDWORD 0
	mainstr1 DWORD ?
	mainstr2 DWORD ?
	mainlen SDWORD 0
	maina SDWORD 0
	mainb SDWORD 0
	mainc SDWORD 0

.code

iszero PROC iszeron : SDWORD
	mov eax, iszeron
	cmp eax, L1
	jz m0
	jnz m1
	je m1
m0:
	push L2
	jmp local0
	jmp e0
m1:
	push L3
	jmp local0
e0:
local0:
	pop eax
	ret
iszero ENDP

count PROC countn : SDWORD
	push L4
	pop counti
	push offset L5

	call OutputStr
	push countn

	call OutputInt
	push offset L6

	call OutputStrLn
	mov eax, counti
	cmp eax, countn
	jl cycle0
	jmp cyclenext0
cycle0:
	push counti

	call OutputInt
	push offset L7

	call OutputStr
	push counti
	push L4
	pop eax
	pop ebx
	add eax, ebx
	push eax
	pop counti
	mov eax, counti
	cmp eax, countn
	jl cycle0
cyclenext0:
	push counti

	call OutputIntLn
	ret
count ENDP

fac PROC faci : SDWORD
	mov eax, faci
	cmp eax, L1
	jz m2
	jnz m3
	je m3
m2:
	push L4
	jmp local1
	jmp e1
m3:
	push faci
	push L4
	pop ebx
	pop eax
	sub eax, ebx
	push eax
	pop facz
	push faci
	push facz
	pop edx
	push facz
	call fac
	push eax
	pop eax
	pop ebx
	mul ebx
	push eax
	jmp local1
e1:
local1:
	pop eax
	ret
fac ENDP

main PROC
	push L8
	pop maini
	push maini

	call OutputInt
	push offset L9

	call OutputStr
	push maini
	pop edx
	push maini
	call iszero
	push eax

	call OutputIntLn
	push offset L7

	call OutputStrLn
	push L10
	call count
	push offset L7

	call OutputStrLn
	push offset L11

	call OutputStr
	push L4
	push L10
	pop edx
	pop edx
	push L10
	push L4
	call mrand
	push eax

	call OutputIntLn
	push offset L7

	call OutputStrLn
	push offset L12
	pop mainstr1
	push offset L13
	pop mainstr2
	push mainstr1
	push mainstr2
	pop edx
	pop edx
	push mainstr2
	push mainstr1
	call scpy
	push eax
	pop mainstr1
	push mainstr1

	call OutputStr
	push offset L14

	call OutputStr
	push mainstr1
	pop edx
	push mainstr1
	call slen
	push eax
	pop mainlen
	push mainlen

	call OutputIntLn
	push offset L7

	call OutputStrLn
	push offset L15

	call OutputStr
	push L8
	push L16
	pop edx
	pop edx
	push L16
	push L8
	call mpow
	push eax

	call OutputIntLn
	push offset L7

	call OutputStrLn
	push offset L17

	call OutputStr
	push L18
	pop edx
	push L18
	call fac
	push eax

	call OutputIntLn
	push offset L7

	call OutputStrLn
	push offset L19

	call OutputStr
	push L20

	call OutputIntLn
	push offset L21

	call OutputStr
	push L22

	call OutputIntLn
	push offset L23

	call OutputStr
	push L24

	call OutputIntLn
	push offset L7

	call OutputStrLn
	push L25
	pop maina
	push L18
	pop mainb
	push L26
	pop mainc
	push maina
	push mainb
	push mainc
	pop eax
	pop ebx
	mul ebx
	push eax
	pop eax
	pop ebx
	add eax, ebx
	push eax
	push L27
	pop ebx
	pop eax
	cdq
	idiv ebx
	push edx
	push L8
	push maina
	push L4
	pop ebx
	pop eax
	sub eax, ebx
	push eax
	pop eax
	pop ebx
	mul ebx
	push eax
	pop ebx
	pop eax
	sub eax, ebx
	push eax
	push L28
	pop eax
	pop ebx
	add eax, ebx
	push eax

	call OutputIntLn
	push 0
	call ExitProcess
main ENDP
end main