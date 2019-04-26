bits 32
section .text

global _start
extrn_main
extrn _exit
extrn __pre_init

_start:
	xor ebp, ebp
	mov esp, stack_top
	call __pre_init
	call main
	push eax
	call _exit
	ud2

section .bss
	resb 4096
stack_top:
