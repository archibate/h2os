bits 32
section .data

globl initImage
globl initImageEnd

initImage:
incbin "../init/init.elf.strip"
initImageEnd:
