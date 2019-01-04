include $Dmake/general.mak

.PHONY: all
all: h2os.img

CLEAN+=h2os.img
h2os.img: kern/kernel.elf

.PHONY: kern/kernel.elf
kern/kernel.elf: libc/libc.a
	make -C $(@D) $(@F)

.PHONY: libc/libc.a
libc/libc.a:
	make -C $(@D) $(@F)

.PHONY: clean
clean:
	rm -rf $(CLEAN)
	make -C kern $@
	make -C libc $@
