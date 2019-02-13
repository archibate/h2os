.PHONY: default
default: all


.PHONY: config
config:
	scripts/config.sh

include tools/config.mak


QEMU=qemu-system-i386
BOCHS=bochs


.PHONY: all
all: out


BASE_PKGNAME=l4 l4env grub

CLEAN+=out
.PHONY: out
out:
	rm -rf $@
	mkdir -p out
	./ipm install $(BASE_PKGNAME)
	rm -rf out/include out/lib

CLEAN+=os.iso
.PHONY: os.iso
os.iso:
	make out
	grub-mkrescue -o $@ out
	rm -rf out

CLEAN+=os.img
.PHONY: os.img
os.img:
	make out
	scripts/mkosimg.sh
	sudo umount isodir || true
	rm -rf isodir
	mkdir -p isodir
	sudo mount os.img isodir
	sudo mv out/* isodir/*
	sudo mv isodir/boot/grub/grub.cfg isodir/menu.lst
	rm -rf out


QEMUFLAGS+=-m 256 $(if $(DEBUG),-S -s,)
QEMUPATHPREFIX=$(if $(MINGW),$D/,)
BOCHSFLAGS+='megs:256'

.PHONY: run
run: out
	$(QEMU) -kernel $(QEMUPATHPREFIX)out/boot/vmlinux-l4 \
		-initrd $(QEMUPATHPREFIX)out/bin/init \
		$(QEMUFLAGS)

.PHONY: runiso
runiso: os.iso
	$(QEMU) -cdrom $(QEMUPATHPREFIX)$< -boot d $(QEMUFLAGS)

.PHONY: bochsiso
bochsiso: os.iso
	$(BOCHS) -q -n 'boot:cdrom' 'ata0-slave: type=cdrom, path=$<, status=inserted' $(BOCHSFLAGS)

.PHONY: runfda
runfda: os.img
	$(QEMU) -fda $(QEMUPATHPREFIX)$< -boot a $(QEMUFLAGS)

.PHONY: bochsfda
bochsfda: os.img
	$(BOCHS) -q -n 'boot:a' 'floppya: 1_44=$<, status=inserted' $(BOCHSFLAGS)


.PHONY: clean
clean:
	rm -rf $(CLEAN)
	make -C src clean
