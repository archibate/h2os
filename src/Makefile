include tools/general.mak

.PHONY: kern/kernel.elf.strip
kern/kernel.elf.strip: libc/libc.a init/init.elf.strip
	make -C $(@D) $(@F)

.PHONY: init/init.elf.strip
init/init.elf.strip: libc/libc.a libl4/libl4.a
	make -C $(@D) $(@F)

.PHONY: libc/libc.a
libc/libc.a:
	make -C $(@D) $(@F)

.PHONY: libc/libl4.a
libl4/libl4.a:
	make -C $(@D) $(@F)

.PHONY: clean
clean:
	rm -rf $(CLEAN)
	make -C kern $@
	make -C init $@
	make -C libc $@
	make -C libl4 $@
