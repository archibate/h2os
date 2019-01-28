bits 32
section .text

globl do_syscall

do_syscall:
%if 0
	int 0x80
	ret
%else
	pushfd
	mov ecx, esp
	mov edx, sysexited
	sysenter
sysexited:
	popfd
	ret
%endif
