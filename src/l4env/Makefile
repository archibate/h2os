include tools/general.mak

.PHONY: default
default: all

SRCS+=$(shell find $(SRCPATH) -regex '.*\.\(c\|asm\)')
OBJS=$(SRCS:%=%.o)
LIBS=c

LDSCRIPT=user.ld

CLEAN+=init.elf $(LDSCRIPT).i $(OBJS)
init.elf: $(LDSCRIPT).i $(OBJS)
	$(EL) ld $@
	$(CC) $(CFLAGS) $(LDFLAGS) -T $(LDSCRIPT).i -o $@ $(OBJS) $(LIBS:%=-l%)

ALL+=out/bin/init
out/bin/init: init.elf
	@mkdir -p $(@D)
ifdef STRIP_ELF
	$(EL) strip $@
	$(STRIP) $(STRIPFLAGS) -o $@ $<
else
	$(EL) inst $@
	cp $< $@
endif

.PHONY: all
all: $(ALL)

.PHONY: clean
clean:
	rm -rf $(CLEAN)

.PHONY: distclean
distclean:
	rm -rf $(CLEAN) out
