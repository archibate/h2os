include $Dmake/general.mak

.PHONY: all
all: kernel.elf

SRCS+=$(shell find src arch/$(ARCH)/src | grep '\.\(c\|asm\)$')
OBJS=$(SRCS:%=%.o)
LIBS=c

LDSCRIPT=arch/$(ARCH)/kernel.ld

CLEAN+=kernel.elf $(LDSCRIPT).i $(OBJS)
kernel.elf: $(LDSCRIPT).i $(OBJS)
	$(EL) ld $@
	$(CC) $(CFLAGS) $(LDFLAGS) -T $(LDSCRIPT).i -o $@ $(OBJS) $(LIBS:%=-l%)

.PHONY: clean
clean:
	rm -rf $(CLEAN)
