bits 32
section .text

globl sysenter_entry
globl seframe_exiter
globl seShortMsg
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
	mov [seShortMsg_EBX], ebx
	mov [seShortMsg_EDI], edi
	mov [seShortMsg_ESI], esi
	mov [seShortMsg_EBP], ebp
	mov ecx, eax
	mov edx, seShortMsg
	call systemCall
seframe_exiter:
	mov ebx, [seShortMsg_EBX]
	mov edi, [seShortMsg_EDI]
	mov esi, [seShortMsg_ESI]
	mov ebp, [seShortMsg_EBP]
	pop es
	pop ds
	pop edx
	pop ecx
	sysexit

section .bss
seShortMsg:
	; N: keep sync with l4/asm/shortmsg.h
seShortMsg_EBX:
	resd 1
seShortMsg_EDI:
	resd 1
seShortMsg_ESI:
	resd 1
seShortMsg_EBP:
	resd 1
