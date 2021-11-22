.586
.model flat, stdcall
includelib libucrt.lib
includelib kernel32.lib
includelib ../Debug/StaticLibrary.lib
ExitProcess PROTO :DWORD

EXTRN BREAKL: proc
EXTRN OutputInt: proc
EXTRN OutputStr: proc
EXTRN OutputIntLn: proc
EXTRN OutputStrLn: proc
EXTRN slen: proc
EXTRN scpy: proc

.stack 4096

.const
	L1 SDWORD 0
	L2 SDWORD 1
	L3 SDWORD 6
	L4 BYTE "Результат = ", 0

.data
	buffer BYTE 256 dup(0)
	facz SDWORD 0
	facres SDWORD 0
	mainres SDWORD 0

.code

fac PROC faci : SDWORD
	mov eax, faci
	cmp eax, L1
	jz m0
	jnz m1
	je m1
m0:
	push 1
	jmp local0
	jmp e0
m1:
	push faci
	push L2
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
	jmp local0
e0:
local0:
	pop eax
	ret
fac ENDP

main PROC
	push L3
	pop edx
	push L3
	call fac
	push eax
	pop mainres
	push offset L4
	call OutputStr
	push mainres
	call OutputIntLn
	call ExitProcess
main ENDP
end main