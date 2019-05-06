bits 32
section .text

global _start
extrn __crt_main

_start:
	xor ebp, ebp
	push esp
	call __crt_main
	ud2
