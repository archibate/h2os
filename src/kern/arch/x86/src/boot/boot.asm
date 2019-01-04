bits 32
section .phys.stub
  jmp _start

section .phys.text

global _start
extern check_mboot
extern setup_boot_vm
extern kern_phys_start

_start:
  cli
  mov esp, stack_top
  xor ebp, ebp
  push eax
  push ebx
  call check_mboot
  add esp, 8
  call setup_boot_vm
  call kern_phys_start
stop:
  hlt
  jmp stop

section .phys.bss
stack: resb 8192
stack_top:
