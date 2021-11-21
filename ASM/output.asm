.586
.model flat, stdcall
includelib libucrt.lib
includelib kernel32.lib
includelib ../Debug/StaticLibrary.lib
ExitProcess PROTO :DWORD

EXTRN BREAKL: proc
EXTRN OutputInt: proc
EXTRN OutputStr: proc

.stack 4096

.const
	L1 SDWORD 0
	L2 SDWORD 10
	L3 SDWORD 1
	L4 SDWORD 5
	L5 SDWORD 20

.data
	buffer BYTE 256 dup(0)
	fii SDWORD 0
	mainout SDWORD 0

.code

fi PROC 
	push L1
	pop fii
	mov eax, fii
	cmp eax, L2
	jl cycle0
	jmp cyclenext0
cycle0:
	push fii
	push L3
	pop eax
	pop ebx
	add eax, ebx
	push eax
	pop fii
	mov eax, fii
	cmp eax, L2
	jl cycle0
cyclenext0:
	mov eax, fii
	cmp eax, L4
	jl m0
	jg m1
	je m1
m0:
	push L5
	pop fii
	jmp e0
m1:
	push L4
	pop fii
e0:
	push fii
	jmp local0
local0:
	pop eax
	ret
fi ENDP

main PROC
	call fi
	push eax
	pop mainout
	push mainout
	call OutputInt
	call ExitProcess
main ENDP
end main