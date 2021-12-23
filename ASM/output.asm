.586
.model flat, stdcall
includelib libucrt.lib
includelib kernel32.lib
includelib ../Debug/StaticLibrary.lib
ExitProcess PROTO :DWORD

EXTRN mpow: proc
EXTRN mrand: proc
EXTRN BREAKL: proc
EXTRN OutputInt: proc
EXTRN OutputStr: proc
EXTRN OutputIntLn: proc
EXTRN OutputStrLn: proc

.stack 4096

.const
	L1 SDWORD 1
	L2 SDWORD 0
	L3 SDWORD 5
	L4 SDWORD 3

.data
	buffer BYTE 256 dup(0)
	powres SDWORD 0
	mainr1 SDWORD 0

.code

pow PROC powa : SDWORD, powe : SDWORD
	push L1
	pop powres
	mov eax, powe
	cmp eax, L2
	jg cycle0
	jmp cyclenext0
cycle0:
	push powres
	push powa
	pop eax
	pop ebx
	add eax, ebx
	push eax
	pop powres
	mov eax, powe
	cmp eax, L2
	jg cycle0
cyclenext0:
	push powres
	jmp local0
local0:
	pop eax
	ret
pow ENDP

main PROC
	push L3
	push L4
	pop edx
	pop edx
	push L4
	push L3
	call pow
	push eax
	pop mainr1
	push mainr1

	call OutputIntLn
	push L3
	push L4
	pop edx
	pop edx
	push L4
	push L3
	call mpow
	push eax

	call OutputIntLn
	push 0
	call ExitProcess
main ENDP
end main