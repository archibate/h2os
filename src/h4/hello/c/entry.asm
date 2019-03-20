bits 32
section .text

global _start
extrn_main
extrn sys_exit

_start:
	xor ebp, ebp
	mov esp, stack_top
	call main
	call sys_exit
	ud2

section .bss
	resb 4096
stack_top:
