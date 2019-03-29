MAKE_A_LIB=1
include $(ROOT)/tools/general.mak

CLEAN+=$(OBJS)
ALL+=out/lib/libc.a
out/lib/libc.a: $(OBJS)
	@mkdir -p $(@D)
	$(EL) ar $@
	$(AR) cqs $@ $(OBJS)

c/str/strerr.c.o: h/errmsgs.gen.inl
h/errmsgs.gen.inl: $(ROOT)/include/errno-bits.h
	$(EC) gen $@
	grep '#define\s\+E' $< | sed 's/\#define\s\+\(\w\+\).*\/\/\s*\(.*\)/\[\1\] = "\2",/' > $@

.PHONY: all
all: $(ALL)

.PHONY: clean
clean:
	rm -rf $(CLEAN)

.PHONY: distclean
distclean:
	rm -rf $(CLEAN) out
