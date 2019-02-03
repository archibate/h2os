include tools/general.mak

.PHONY: default
default: all

ALL+=out/boot/grub/grub.cfg
out/boot/grub/grub.cfg: grub.cfg
	@mkdir -p $(@D)
	cp $< $@

.PHONY: all
all: $(ALL)

.PHONY: clean
clean:
	rm -rf $(CLEAN)

.PHONY: distclean
distclean:
	rm -rf $(CLEAN) out
