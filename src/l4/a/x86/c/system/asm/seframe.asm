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
        mov esi, [_systab + eax * 4]
	call sched_enter
	call esi
	add esp, 16
	push eax
	call sched_leave
seframe_exiter:
	pop eax
.sexit_noeax:
	pop es
	pop ds
	pop edx
	pop ecx
	sti
	sysexit
