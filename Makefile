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


BASE_PKGNAME=base

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


MEGS=256
Q=$(if $(MINGW),$D/,)
QEMUFLAGS+=-m $(MEGS) $(if $(DEBUG),-S -s,)
BOCHSFLAGS+='megs:$(MEGS)'

L4ENV_MODS=$(shell cat src/l4env/package.ini | grep deps= | sed 's/deps=//' | sed 's/l4env-//g')
QINITRD=$(shell echo $(L4ENV_MODS:%=$Qout/bin/%) | awk '{for (i=1;i<=NF;i++)printf "%s%s",$$i,(i!=NF?",":"");}')

.PHONY: showinfo
showinfo:
	@echo QEMU=$(QEMU)
	@echo BOCHS=$(BOCHS)
	@echo MINGW=$(MINGW)
	@echo L4ENV_MODS=$(L4ENV_MODS)
	@echo BASE_PKGNAME=$(BASE_PKGNAME)
	@echo QEMUFLAGS=$(QEMUFLAGS)
	@echo BOCHSFLAGS=$(BOCHSFLAGS)
	@echo QINITRD=$(QINITRD)
	@echo MEGS=$(MEGS)

.PHONY: run
run: out
	$(QEMU) -kernel $Qout/boot/vmlinux-l4 \
		-initrd "$(QINITRD)" \
		$(QEMUFLAGS)

.PHONY: runiso
runiso: os.iso
	$(QEMU) -cdrom $Q$< -boot d $(QEMUFLAGS)

.PHONY: bochsiso
bochsiso: os.iso
	$(BOCHS) -q -n 'boot:cdrom' 'ata0-slave: type=cdrom, path=$<, status=inserted' $(BOCHSFLAGS)

.PHONY: runfda
runfda: os.img
	$(QEMU) -fda $Q$< -boot a $(QEMUFLAGS)

.PHONY: bochsfda
bochsfda: os.img
	$(BOCHS) -q -n 'boot:a' 'floppya: 1_44=$<, status=inserted' $(BOCHSFLAGS)


.PHONY: clean
clean:
	rm -rf $(CLEAN)

.PHONY: distclean
distclean:
	rm -rf $(CLEAN) out
