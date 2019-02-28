MAKE_A_LIB=1
include $(ROOT)/tools/general.mak

.PHONY: default
default: all

SRCS+=$(shell find $(SRCPATH) $(FINDFLAGS))
OBJS=$(SRCS:%=%.o)

CLEAN+=$(OBJS)
ALL+=out/lib/libh4.a
out/lib/libh4.a: $(OBJS)
	@mkdir -p $(@D)
	$(EL) ar $@
	@#cp -r $(ROOT)/out/lib/libc.a $@
	$(AR) cqs $@ $(OBJS)

ALL+=out/include/h4
out/include/h4: $(ADIRS:%=%/h/h4)
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
