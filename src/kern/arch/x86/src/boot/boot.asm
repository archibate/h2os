bits 32
section .stub
  jmp _start

section .text

globl _start
extrn check_mboot
extrn setup_paging
extrn kern_phys_start

_start:
  cli
  mov byte [0xb8000], 'O'
  mov byte [0xb8002], 'K'
  mov esp, stack_top
  xor ebp, ebp
  push eax
  push ebx
  call check_mboot
  add esp, 8
  call setup_paging
  call kern_phys_start
stop:
  hlt
  jmp stop

section .bss
stack: resb 8192
stack_top:
