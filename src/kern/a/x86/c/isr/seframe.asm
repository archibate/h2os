bits 32
section .text

globl sysenter_entry
extrn systemCall

sysenter_entry:
	cli
	push ecx
	push edx
	push ds
	push es
	mov edx, ss
	mov ds, edx
	mov es, edx
	; N: keep sync with libl4/invoke.h & l4/syscall.c
	mov [invo_service], eax
	mov [invo_offset], esi
	mov [invo_length], edi
	mov [invo_dataptr], ebp
	mov edx, invo
	mov ecx, ebx
	call systemCall
	pop es
	pop ds
	pop edx
	pop ecx
	sysexit

section .bss
invo:
	; N: keep sync with l4/invoke.h
invo_service:
	resd 1
invo_offset:
	resd 1
invo_length:
	resd 1
invo_dataptr:
	resd 1
