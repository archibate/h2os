export D=$(shell pwd)/
#export MINGW=1
export ARCH=x86

ifndef MINGW
QEMU=qemu-system-i386
else
QEMU=C:\Users\Lenovo\Intel\OSD\QEMU\qemu-system-i386w.exe
#QEMU=./tools/qemu.bat
#QEMU=./tools/qemu/qemu-win.bat
endif

CLEAN+=Image
.PHONY: Image
Image:
	rm -f $@
	make -C src all
	ln -s src/kern/kernel.elf $@

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
run: Image
	$(QEMU) -kernel $(if $(MINGW),$(PWD)/,)$<

.PHONY: clean
clean:
	rm -rf $(CLEAN)
	make -C src clean
