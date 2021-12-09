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
	L10 BYTE "true", 0
	L11 BYTE "false", 0
	L12 SDWORD 10
	L13 BYTE "Случайное число от 1 до 10: ", 0
	L14 BYTE "Александр", 0
	L15 BYTE "Артем", 0
	L16 BYTE ", длина = ", 0
	L17 SDWORD 3
	L18 BYTE "5^3 = ", 0
	L19 SDWORD 6
	L20 BYTE "6! = ", 0
	L21 BYTE "0xfff = ", 0
	L22 SDWORD 4095
	L23 BYTE "0x14de = ", 0
	L24 SDWORD 5342
	L25 BYTE "0x2ea = ", 0
	L26 SDWORD 746
	L27 SDWORD 4
	L28 SDWORD 14
	L29 SDWORD 7

.data
	buffer BYTE 256 dup(0)
	counti SDWORD 0
	facz SDWORD 0
	facres SDWORD 0
	maini SDWORD 0
	mainis SDWORD 0
	mainr SDWORD 0
	mainstr1 DWORD ?
	mainstr2 DWORD ?
	mainlen SDWORD 0
	mainpow SDWORD 0
	mainres SDWORD 0
	maina SDWORD 0
	mainb SDWORD 0
	mainc SDWORD 0
	mainexpr SDWORD 0

.code

iszero PROC iszeron : SDWORD
	mov eax, iszeron
	cmp eax, L1
	jz m0
	jnz m1
	je m1
m0:
	push 1
	jmp local0
	jmp e0
m1:
	push 0
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
	push 1
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
	pop facres
	push facres
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
	pop edx
	push maini
	call iszero
	push eax
	pop mainis
	push maini
	call OutputInt
	push offset L9
	call OutputStr
	mov eax, mainis
	cmp eax, 1
	jz m4
	jnz m5
	je m5
m4:
	push offset L10
	call OutputStrLn
	jmp e2
m5:
	push offset L11
	call OutputStrLn
e2:
	push offset L7
	call OutputStrLn
	push L12
	call count
	push offset L7
	call OutputStrLn
	push L4
	push L12
	pop edx
	pop edx
	push L12
	push L4
	call mrand
	push eax
	pop mainr
	push offset L13
	call OutputStr
	push mainr
	call OutputIntLn
	push offset L7
	call OutputStrLn
	push offset L14
	pop mainstr1
	push offset L15
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
	push mainstr1
	pop edx
	push mainstr1
	call slen
	push eax
	pop mainlen
	push offset L16
	call OutputStr
	push mainlen
	call OutputIntLn
	push offset L7
	call OutputStrLn
	push L8
	push L17
	pop edx
	pop edx
	push L17
	push L8
	call mpow
	push eax
	pop mainpow
	push offset L18
	call OutputStr
	push mainpow
	call OutputIntLn
	push offset L7
	call OutputStrLn
	push L19
	pop edx
	push L19
	call fac
	push eax
	pop mainres
	push offset L20
	call OutputStr
	push mainres
	call OutputIntLn
	push offset L7
	call OutputStrLn
	push offset L21
	call OutputStr
	push L22
	call OutputIntLn
	push offset L23
	call OutputStr
	push L24
	call OutputIntLn
	push offset L25
	call OutputStr
	push L26
	call OutputIntLn
	push offset L7
	call OutputStrLn
	push L27
	pop maina
	push L19
	pop mainb
	push L28
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
	push L29
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
	pop mainexpr
	push mainexpr
	call OutputIntLn
	call ExitProcess
main ENDP
end main