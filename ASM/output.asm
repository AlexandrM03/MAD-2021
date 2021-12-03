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
	L1 SDWORD 5
	L2 BYTE "5^3 = ", 0
	L3 BYTE "Случайное число: ", 0
	L4 SDWORD 1
	L5 BYTE "1234567h#jknf", 0
	L6 BYTE "Длина строки: ", 0

.data
	buffer BYTE 256 dup(0)
	maini SDWORD 0
	mainr SDWORD 0
	mainstr DWORD ?
	mainlen SDWORD 0

.code

main PROC
	push L1
	pop maini
	pop maini
	push offset L2
	call OutputStr
	push maini
	call OutputIntLn
	push offset L3
	call OutputStr
	push L4
	push L1
	pop edx
	pop edx
	push L1
	push L4
	call mrand
	push eax
	pop mainr
	push mainr
	call OutputIntLn
	push offset L5
	pop mainstr
	push mainstr
	pop edx
	push mainstr
	call slen
	push eax
	pop mainlen
	push offset L6
	call OutputStr
	push mainlen
	call OutputInt
	call ExitProcess
main ENDP
end main