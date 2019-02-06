#!/usr/bin/cgdb -x
# vim: ft=gdb ts=2 sts=2 sw=2 fdm=marker
# https://github.com/archibate/OS/blob/master/.gdbinit

# Setting up QEMU & GDB... {{{

# For bootloader debugging
# change architecture to i386 if debugging STAGE2 32-bit code
#set architecture i8086
set architecture i386
#break *0x7C00
#break *0x10000000
#delete 1

# For both ;)
set disassemble-next-line on
set disassembly-flavor intel
# }}}
# Some useful macros {{{

# Connect to QEMU
define qemu
target remote localhost:1234
end

# For kernel debugging
# Add KERNEL file for debugging information
define asfu
	add-symbol-file src/$arg0/out/bin/$arg0 0x10000000
end

define asfk
	add-symbol-file src/kern/vmlinux.elf 0x100000
end

define qq
	kill
	quit
end

define bc
	break $arg0
	continue
end
# }}}
# Some specific macros {{{
define xs
	x/6wx $esp
end

define xsi
	x/10i *(void**)($esp+4)
end
# }}}

qemu
asfk
asfu init

set $utcb = (utcb_t*)0x3fd000
