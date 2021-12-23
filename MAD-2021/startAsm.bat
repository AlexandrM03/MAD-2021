@echo off
call D:\VS\VC\Auxiliary\Build\vcvarsall.bat x86
ml /c /coff /Zi "D:\2course\MAD-2021\MAD-2021\program.txt.out.asm"
link /OPT:NOREF /DEBUG /SUBSYSTEM:CONSOLE libucrt.lib "D:\2course\MAD-2021\MAD-2021\program.txt.out.obj"
program.txt.out.exe
pause 