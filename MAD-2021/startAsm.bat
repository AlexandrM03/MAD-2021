@echo off
call D:\VS\VC\Auxiliary\Build\vcvarsall.bat x86
ml /c /coff /Zi program.txt.out.asm
link /OPT:NOREF /DEBUG /SUBSYSTEM:CONSOLE libucrt.lib program.txt.out.obj 
program.txt.out.exe
pause 