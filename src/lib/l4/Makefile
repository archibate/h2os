MAKE_A_LIB=1
include $(ROOT)/tools/general.mak

CLEAN+=$(OBJS)
ALL+=out/lib/libl4.a
out/lib/libl4.a: $(OBJS)
	@mkdir -p $(@D)
	$(EL) ar $@
	@#cp -r $(ROOT)/out/lib/libc.a $@
	$(AR) cqs $@ $(OBJS)

ALL+=out/include/l4
out/include/l4: $(ADIRS:%=%/h/l4)
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
