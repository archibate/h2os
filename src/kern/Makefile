DEFINES+=_KERNEL=1
DEFINES+=CONFIG_DEBUG_LOG=1 CONFIG_DEBUG_OBJECT=1 CONFIG_CONSOLE_CLEAR=1
include tools/general.mak

.PHONY: all
all: kernel.elf.strip

SRCS+=$(shell find $(SRCPATH) -regex '.*\.\(c\|asm\)') sc/nametab.gen.c
OBJS=$(SRCS:%=%.o)
LIBS=c

LDSCRIPT=a/$(ARCH)/kernel.ld

CLEAN+=kernel.elf $(LDSCRIPT).i $(OBJS)
kernel.elf: $(LDSCRIPT).i $(OBJS)
	$(EL) ld $@
	$(CC) $(CFLAGS) $(LDFLAGS) -T $(LDSCRIPT).i -o $@ $(OBJS) $(LIBS:%=-l%)

sc/nametab.gen.c: $(wildcard sh/l4/*.h)
	tools/nametab.sh > $@

c/init/initimg.asm.o: ../init/init.elf.strip

.PHONY: clean
clean:
	rm -rf $(CLEAN)
