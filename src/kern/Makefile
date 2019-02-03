DEFINES+=_KERNEL=1
DEFINES+=CONFIG_DEBUG_LOG=1 CONFIG_DEBUG_OBJECT=1 CONFIG_CONSOLE_CLEAR=1
include tools/general.mak

.PHONY: default
default: all

SRCS+=$(shell find $(SRCPATH) -regex '.*\.\(c\|asm\)') sc/nametab.gen.c
OBJS=$(SRCS:%=%.o)
LIBS=c

LDSCRIPT=a/$(ARCH)/kernel.ld

CLEAN+=vmlinux.elf $(LDSCRIPT).i $(OBJS)
vmlinux.elf: $(LDSCRIPT).i $(OBJS)
	$(EL) ld $@
	$(CC) $(CFLAGS) $(LDFLAGS) -T $(LDSCRIPT).i -o $@ $(OBJS) $(LIBS:%=-l%)

ALL+=out/boot/vmlinux-l4
out/boot/vmlinux-l4: vmlinux.elf
	@mkdir -p $(@D)
	$(EL) strip $@
	$(STRIP) $(STRIPFLAGS) -o $@ $<

sc/nametab.gen.c: $(wildcard sh/l4/*.h)
	tools/nametab.sh > $@

.PHONY: all
all: $(ALL)

.PHONY: clean
clean:
	rm -rf $(CLEAN)

.PHONY: distclean
distclean:
	rm -rf $(CLEAN) out
