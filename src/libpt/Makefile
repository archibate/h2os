MAKE_A_LIB=1
include $(ROOT)/tools/general.mak

CLEAN+=$(OBJS)
ALL+=out/lib/libl4.a
out/lib/libl4.a: $(OBJS)
	@mkdir -p $(@D)
	$(EL) ar $@
	$(AR) cqs $@ $(OBJS)

.PHONY: all
all: $(ALL)

.PHONY: clean
clean:
	rm -rf $(CLEAN)

.PHONY: distclean
distclean:
	rm -rf $(CLEAN) out
