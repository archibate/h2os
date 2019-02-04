.PHONY: default
default: all


.PHONY: config
config:
	scripts/config.sh

include tools/config.mak


QEMU=qemu-system-i386


.PHONY: all
all: isodir


CLEAN+=isodir
.PHONY: isodir
isodir:
	sudo umount isodir || true
	rm -rf $@
	mkdir -p isodir
	./ipm install base

CLEAN+=os.iso
.PHONY: os.iso
os.iso:
	sudo umount isodir || true
	rm -rf $@
	mkdir -p isodir
	./ipm install base
	grub-mkrescue -o $@ isodir

CLEAN+=os.img
.PHONY: os.img
os.img:
	sudo umount isodir || true
	rm -rf $@
	mkdir -p isodir
	scripts/mkosimg.sh
	sudo mount os.img isodir
	ipm_sudo=sudo ./ipm install base
	sudo mv isodir/boot/grub/grub.cfg isodir/menu.lst


QEMUFLAGS+=-m 256 $(if $(DEBUG),-S -s,)
QEMUPATHPREFIX=$(if $(MINGW),$D/,)

.PHONY: run
run: isodir
	$(QEMU) -kernel $(QEMUPATHPREFIX)isodir/boot/vmlinux-l4 \
		-initrd $(QEMUPATHPREFIX)isodir/bin/init \
		$(QEMUFLAGS)

.PHONY: runiso
runiso: os.iso
	$(QEMU) -cdrom $(QEMUPATHPREFIX)$< -boot d $(QEMUFLAGS)

.PHONY: runfda
runfda: os.img
	$(QEMU) -fda $(QEMUPATHPREFIX)$< -boot a $(QEMUFLAGS)


.PHONY: clean
clean:
	rm -rf $(CLEAN)
	make -C src clean
