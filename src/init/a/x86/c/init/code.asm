bits 32
section .text

globl testcode_start
globl testcode_end

testcode_start:
	jmp testcode_start

testcode_end
