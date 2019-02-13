bits 32
section .text

globl sysenter_entry
globl seframe_exiter
extrn sched_enter
extrn sched_leave
extrn _systab

sysenter_entry:
	cli
	push ecx
	push edx
	push ds
	push es
	mov edx, ss
	mov ds, edx
	mov es, edx
	push ebp
	push esi
	push edi
	push ebx
	push eax
	call sched_enter
	pop eax
	call [_systab + eax * 4]
	push eax
	call sched_leave
	pop eax
	add esp, 16
seframe_exiter:
	pop es
	pop ds
	pop edx
	pop ecx
	sti
	sysexit
