bits 32
section .text

globl _start
extrn main

_start:
	jmp _start
