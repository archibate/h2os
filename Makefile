.PHONY: default
default: all


.PHONY: config
config:
	scripts/config.sh

include tools/config.mak

ifndef QEMU
QEMU=qemu-system-i386
endif
ifndef BOCHS
BOCHS=bochs
endif
IPM=scripts/ipm.sh


.PHONY: all
all: out


ifdef MINGW
BASE_PKGNAME=libgcc base
else
BASE_PKGNAME=base
endif

CLEAN+=out
.PHONY: out
out:
	rm -rf $@
	mkdir -p out
	$(IPM) install $(BASE_PKGNAME)
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
HDA_IMG=hda.img
QEMUFLAGS+=-m $(MEGS) $(if $(DEBUG),-S -s,) -hda $(HDA_IMG)
BOCHSFLAGS+='megs:$(MEGS)' 'ata0-master: type=disk, path="$(HDA_IMG)", mode=flat'

H4_MODS=$(shell cat src/h4/package.ini | grep deps= | sed 's/deps=//' | sed 's/h4-//g')
QINITRD=$(shell echo $(H4_MODS:%=out/bin/%) | awk '{for (i=1;i<=NF;i++)printf "%s%s",$$i,(i!=NF?",":"");}')

.PHONY: showinfo
showinfo:
	@echo QEMU=$(QEMU)
	@echo BOCHS=$(BOCHS)
	@echo MINGW=$(MINGW)
	@echo H4_MODS=$(H4_MODS)
	@echo BASE_PKGNAME=$(BASE_PKGNAME)
	@echo QEMUFLAGS=$(QEMUFLAGS)
	@echo BOCHSFLAGS=$(BOCHSFLAGS)
	@echo QINITRD=$(QINITRD)
	@echo MEGS=$(MEGS)

.PHONY: run
run: out
	$(QEMU) -kernel out/boot/vmlinux-l4 \
		-initrd "$(QINITRD)" \
		$(QEMUFLAGS)

.PHONY: runiso
runiso: os.iso
	$(QEMU) -cdrom $< -boot d $(QEMUFLAGS)

.PHONY: bochsiso
bochsiso: os.iso
	$(BOCHS) -q -n 'boot:cdrom' 'ata0-slave: type=cdrom, path=$<, status=inserted' $(BOCHSFLAGS)

.PHONY: runfda
runfda: os.img
	$(QEMU) -fda $< -boot a $(QEMUFLAGS)

.PHONY: bochsfda
bochsfda: os.img
	$(BOCHS) -q -n 'boot:a' 'floppya: 1_44=$<, status=inserted' $(BOCHSFLAGS)


.PHONY: clean
clean:
	rm -rf $(CLEAN)

.PHONY: distclean
distclean:
	rm -rf $(CLEAN) out
