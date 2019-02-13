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
	test esi, esi
	jz serr
	call sched_enter
	call esi
	add esp, 16
	push eax
	call sched_leave
seframe_exiter:
	pop eax
	pop es
	pop ds
	pop edx
	pop ecx
	sti
	sysexit
serr:
	mov byte [0xb8000], 's'
	mov byte [0xb8002], 'E'
	mov byte [0xb8004], 'R'
	mov byte [0xb8006], 'R'
	cli
	hlt
