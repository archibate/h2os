bits 32
section .text

global _start
extrn __crt_main

_start:
	xor ebp, ebp
	mov esp, stack_top
	call __crt_main
	ud2

section .bss
	resb 4096
stack_top:
