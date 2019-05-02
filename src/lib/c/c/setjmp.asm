; vim: ft=nasm ai

section .text
bits 32

globl setjmp
setjmp:
	mov ecx, [esp + 4]
	mov [ecx + 0], ebx
	mov [ecx + 4], edi
	mov [ecx + 8], esi
	mov [ecx + 12], ebp
	mov edx, [esp + 0]
	mov [ecx + 16], edx
	mov [ecx + 20], esp
	ret

globl longjmp
longjmp:
	mov ecx, [esp + 4]
	mov ebx, [ecx + 0]
	mov edi, [ecx + 4]
	mov esi, [ecx + 8]
	mov ebp, [ecx + 12]
	mov edx, [ecx + 16]
	mov esp, [ecx + 20]
	jmp edx
