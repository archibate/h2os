include $(ROOT)/tools/general.mak

.PHONY: default
default: all

ALL+=out/lib/libgcc.a
out/lib/libgcc.a: $(OBJS)
	@mkdir -p $(@D)
	$(EL) inst $@
	cp $(ROOT)/../../../../lib/gcc/mingw32/3.4.5/libgcc.a $@

.PHONY: all
all: $(ALL)

.PHONY: clean
clean:
	rm -rf $(CLEAN)

.PHONY: distclean
distclean:
	rm -rf $(CLEAN) out
