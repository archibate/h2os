include tools/general.mak

.PHONY: default
default: all

SRCS+=$(shell find $(SRCPATH) -regex '.*\.\(c\|asm\)')
OBJS=$(SRCS:%=%.o)
LIBS=l4 c

LDSCRIPT=a/$(ARCH)/init.ld

CLEAN+=$(LDSCRIPT).i $(OBJS)
ALL+=out/bin/init
out/bin/init: $(LDSCRIPT).i $(OBJS)
	@mkdir -p $(@D)
	$(EL) ld $@
	$(CC) $(CFLAGS) $(LDFLAGS) -T $(LDSCRIPT).i -o $@ $(OBJS) $(LIBS:%=-l%)

.PHONY: all
all: $(ALL)

.PHONY: clean
clean:
	rm -rf $(CLEAN)

.PHONY: distclean
distclean:
	rm -rf $(CLEAN) out
