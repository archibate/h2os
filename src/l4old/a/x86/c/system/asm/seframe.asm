bits 32
section .text

globl sysenter_entry
globl seframe_exiter
extrn hwsysenter

sysenter_entry:
	cli
	push ecx
	push edx
	push ds
	push es
	mov edx, ss
	mov ds, edx
	mov es, edx
	call hwsysenter
seframe_exiter:
	pop es
	pop ds
	pop edx
	pop ecx
	sysexit
