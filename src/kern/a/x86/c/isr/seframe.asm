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
	mov [ShortMsg_EBX], ebx
	mov [ShortMsg_EDI], edi
	mov [ShortMsg_ESI], esi
	mov [ShortMsg_EBP], ebp
	mov ecx, eax
	mov edx, ShortMsg
	call systemCall
	pop es
	pop ds
	pop edx
	pop ecx
	sysexit

section .bss
ShortMsg:
	; N: keep sync with l4/asm/shortmsg.h
ShortMsg_EBX:
	resd 1
ShortMsg_EDI:
	resd 1
ShortMsg_ESI:
	resd 1
ShortMsg_EBP:
	resd 1
