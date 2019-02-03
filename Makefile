.PHONY: default
default: all


.PHONY: config
config:
	scripts/config.sh

include tools/config.mak


QEMU=qemu-system-i386


.PHONY: all
all: out


CLEAN+=os.iso
os.iso: isodir
	grub-mkrescue -o $@ isodir

CLEAN+=isodir
.PHONY: isodir
isodir:
	rm -rf $@
	./ipm install base


QEMUFLAGS+=-m 256 $(if $(DEBUG),-S -s,)
QEMUPATHPREFIX=$(if $(MINGW),$D/,)

.PHONY: run
run: isodir
	$(QEMU) -kernel $(QEMUPATHPREFIX)isodir/boot/vmlinux-l4 -initrd $(QEMUPATHPREFIX)isodir/bin/init $(QEMUFLAGS)

.PHONY: runiso
runiso: os.iso
	$(QEMU) -cdrom $(QEMUPATHPREFIX)os.iso -boot d $(QEMUFLAGS)


.PHONY: clean
clean:
	rm -rf $(CLEAN)
	make -C src clean
