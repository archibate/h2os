bits 32
section .text

global _start
extrn_main

_start:
	xor ebp, ebp
	mov esp, stack_top
	call main
	jmp $

section .bss
	resb 4096
stack_top:
