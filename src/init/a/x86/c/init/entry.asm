bits 32
section .stub
	resd 19 ;IFrameWords
	jmp _start

section .text

globl _start
extrn init_main

_start:
	mov esp, stack_top
	xor ebp, ebp

	call init_main
stop:
	hlt
	jmp stop

section .bss
stack: resb 8192
stack_top:
