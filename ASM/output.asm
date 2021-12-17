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

.stack 4096

.const
	L1 SDWORD 1
	L2 SDWORD 5
	L3 BYTE "Привет", 0
	L4 SDWORD 1

.data
	buffer BYTE 256 dup(0)
	printprintg SDWORD 0
	maina SDWORD 0
	mains DWORD ?
	mainb SDWORD 0

.code

print PROC printa : SDWORD, prints : DWORD, printb : SDWORD
	push L1
	pop printprintg
	push printa
	call OutputIntLn
	push prints
	call OutputStrLn
	push printb
	call OutputIntLn
	ret
print ENDP

main PROC
	push L2
	pop maina
	push offset L3
	pop mains
	push L4
	pop mainb
	push mainb
	push mains
	push maina
	call print
	call ExitProcess
main ENDP
end main