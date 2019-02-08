bits 32
section .text

globl testcode_start
globl testcode_end

testcode_start:
	int 0x81
	jmp testcode_start

testcode_end
