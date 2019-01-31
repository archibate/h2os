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


CLEAN+=h2os.iso
h2os.iso: isodir
	grub-mkrescue -o $@ isodir

CLEAN+=isodir
.PHONY: isodir
isodir: scripts/grub.cfg
	mkdir -p $@/boot/grub
	cp scripts/grub.cfg $@/boot/grub
	cp vmlinux $@/boot


QEMUFLAGS+=-m 256 $(if $(DEBUG),-S -s,)
QEMUPATHPREFIX=$(if $(MINGW),$D/,)

.PHONY: run
run: vmlinux
	$(QEMU) -kernel $(QEMUPATHPREFIX)$< $(QEMUFLAGS)

.PHONY: runiso
runiso: h2os.iso
	$(QEMU) -cdrom $(QEMUPATHPREFIX)$< -boot d $(QEMUFLAGS)


.PHONY: clean
clean:
	rm -rf $(CLEAN)
	make -C src clean
