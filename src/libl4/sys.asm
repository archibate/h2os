bits 32
section .text

extrn libl4_do_syscall
globl sys_action

sys_action:
	mov e
