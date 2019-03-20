bits 32
section .text

globl libl4_do_syscall

libl4_do_syscall:
%if 0
	int 0x80
	ret
%else
	pushfd
	mov ecx, esp
	call getpc_edx
	add edx, sysexited-$
	sysenter
sysexited:
	popfd
	ret
getpc_edx:
	mov edx, [esp]
	ret
%endif
