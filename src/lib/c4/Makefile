MAKE_A_LIB=1
include $(ROOT)/tools/general.mak

CLEAN+=$(OBJS)
ALL+=out/lib/libc4.a
out/lib/libc4.a: $(OBJS)
	@mkdir -p $(@D)
	$(EL) ar $@
	@#cp -r $(ROOT)/out/lib/libc.a $@
	$(AR) cqs $@ $(OBJS)

ALL+=out/lib/crt0.o
out/lib/crt0.o: crt/crt0.asm.o
	@mkdir -p $(@D)
	$(EL) cop $@
	cp $< $@

ALL+=out/lib/crt1.o
out/lib/crt1.o: crt/crt1.c.o
	@mkdir -p $(@D)
	$(EL) cop $@
	cp $< $@

ALL+=out/include/c4
out/include/c4: $(ADIRS:%=%/h/c4)
	@mkdir -p $(@D)
	@rm -rf $@
	cp -r $^ $(@D)

.PHONY: all
all: $(ALL)

.PHONY: clean
clean:
	rm -rf $(CLEAN)

.PHONY: distclean
distclean:
	rm -rf $(CLEAN) out
