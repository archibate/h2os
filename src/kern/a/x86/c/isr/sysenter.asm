bits 32
section .text

globl sysenter_entry
extrn hwsyscall

sysenter_entry:
	cli
	push ecx
	push edx
	push ds
	push es
	mov edx, ss
	mov ds, edx
	mov es, edx
	call hwsyscall
	pop es
	pop ds
	pop edx
	pop ecx
	sysexit
