export D=$(PWD)/

CLEAN+=Image
.PHONY: Image
Image:
	rm -f $@
	make -C c all
	ln -s c/kern/kernel.elf $@

CLEAN+=isodir
.PHONY: isodir
isodir:
	mkdir -p isodir/boot

CLEAN+=h2os.iso
h2os.iso: isodir
	grub2-mkrescue -o $@ isodir

.PHONY: runiso
runiso: h2os.iso
	qemu-system-i386 -cdrom $<

.PHONY: run
run: Image
	qemu-system-i386 -kernel $<

.PHONY: clean
clean:
	rm -rf $(CLEAN)
	make -C c clean
