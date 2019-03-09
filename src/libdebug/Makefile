MAKE_A_LIB=1
include $(ROOT)/tools/general.mak

CLEAN+=$(OBJS)
ALL+=out/lib/libdebug.a
out/lib/libdebug.a: $(OBJS)
	@mkdir -p $(@D)
	$(EL) ar $@
	@#cp -r $(ROOT)/out/lib/libc.a $@
	$(AR) cqs $@ $(OBJS)

.PHONY: all
all: $(ALL)

.PHONY: clean
clean:
	rm -rf $(CLEAN)

.PHONY: distclean
distclean:
	rm -rf $(CLEAN) out
