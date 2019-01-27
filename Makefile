export D=$(shell pwd)/
#export MINGW=1
export ARCH=x86

ifndef MINGW
QEMU=qemu-system-i386
else
QEMU=C:\Users\Lenovo\Intel\OSD\QEMU\qemu-system-i386w.exe
endif

CLEAN+=vmlinux
.PHONY: vmlinux
vmlinux:
	rm -f $@
	make -C src kern/kernel.elf.strip
	ln -s src/kern/kernel.elf.strip $@

CLEAN+=isodir
.PHONY: isodir
isodir: $Dscripts/grub.cfg
	mkdir -p isodir/boot/grub
	cp $Dscripts/grub.cfg

CLEAN+=h2os.iso
h2os.iso: isodir
	grub2-mkrescue -o $@ isodir

.PHONY: runiso
runiso: h2os.iso
	$(QEMU) -cdrom $(if $(MINGW),$(PWD)/,)$<

.PHONY: run
run: vmlinux
	$(QEMU) -kernel $(if $(MINGW),$(PWD)/,)$<

.PHONY: clean
clean:
	rm -rf $(CLEAN)
	make -C src clean
