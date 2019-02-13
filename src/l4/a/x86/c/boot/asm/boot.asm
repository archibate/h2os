bits 32
section .stub
	jmp _start

section .text

global _start
extrn save_mboot_pointer
extrn arch_startup

_start:
	cli
	mov byte [0xb8000], 'O'
	mov byte [0xb8002], 'K'

	mov esp, stack_top
	xor ebp, ebp

	push eax
	push ebx
	call save_mboot_pointer
	add esp, 8

	call arch_startup
stop:
	hlt
	jmp stop

section .bss
stack: resb 8192
stack_top:
