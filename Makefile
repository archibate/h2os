export D=$(shell pwd)/
#export MINGW=1
export ARCH=x86


ifndef MINGW
QEMU=qemu-system-i386
else
QEMU=C:\Users\Lenovo\Intel\OSD\QEMU\qemu-system-i386w.exe
endif


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
	grub2-mkrescue -o $@ isodir

CLEAN+=isodir
.PHONY: isodir
isodir: $Dscripts/grub.cfg
	mkdir -p isodir/boot/grub
	cp $Dgrub.cfg


QEMUFLAGS+=-m 256 $(if $(DEBUG),-S -s,)
QEMUPATHPREFIX=$(if $(MINGW),$D/,)

.PHONY: run
run: vmlinux
	$(QEMU) -kernel $(QEMUPATHPREFIX)$< $(QEMUFLAGS)

.PHONY: runiso
runiso: h2os.iso
	$(QEMU) -cdrom $(QEMUPATHPREFIX)$< $(QEMUFLAGS)


.PHONY: clean
clean:
	rm -rf $(CLEAN)
	make -C src clean
