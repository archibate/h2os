#CROSS=/opt/i686-elf/bin/i686-elf-

ifeq ($(ARCH),x86)
ASM=nasm
endif
AS=$(CROSS)as
CC=$(CROSS)gcc
LD=$(CROSS)ld
AR=$(CROSS)ar
STRIP=$(CROSS)strip
