.PHONY: default
default: all


.PHONY: config
config:
	scripts/config.sh

include tools/config.mak


QEMU=qemu-system-i386


.PHONY: all
all: vmlinux


CLEAN+=vmlinux
.PHONY: vmlinux
vmlinux:
	rm -f $@
	make -C src kern/kernel.elf.strip
	ln -s src/kern/kernel.elf.strip $@


CLEAN+=initrd
.PHONY: initrd
initrd:
	rm -f $@
	make -C src init/init.elf.strip
	ln -s src/init/init.elf.strip $@


CLEAN+=os.iso
os.iso: isodir
	grub-mkrescue -o $@ isodir

CLEAN+=isodir
.PHONY: isodir
isodir: scripts/grub.cfg vmlinux
	rm -rf $@
	mkdir -p $@/boot/grub
	cp scripts/grub.cfg $@/boot/grub
	cp `readlink vmlinux` $@/boot/vmlinux-l4-$(PLAT)
	cp `readlink initrd` $@/boot/initrd-l4env-$(PLAT)


QEMUFLAGS+=-m 256 $(if $(DEBUG),-S -s,)
QEMUPATHPREFIX=$(if $(MINGW),$D/,)

.PHONY: run
run: vmlinux initrd
	$(QEMU) -kernel $(QEMUPATHPREFIX)vmlinux -initrd "initrd,README.md" $(QEMUFLAGS)

.PHONY: runiso
runiso: os.iso
	$(QEMU) -cdrom $(QEMUPATHPREFIX)os.iso -boot d $(QEMUFLAGS)


.PHONY: clean
clean:
	rm -rf $(CLEAN)
	make -C src clean
