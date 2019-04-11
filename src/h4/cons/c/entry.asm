bits 32
section .text

global _start
extrn_main
extrn _exit
extrn __libh4_init

_start:
	xor ebp, ebp
	mov esp, stack_top
	call __libh4_init
	call main
	call _exit
	ud2

section .bss
	resb 4096
stack_top:
