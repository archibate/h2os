include tools/general.mak

.PHONY: all
all: init.elf.strip

SRCS+=$(shell find c a/$(ARCH)/c | grep '\.\(c\|asm\)$$')
OBJS=$(SRCS:%=%.o)
LIBS=c sys

LDSCRIPT=a/$(ARCH)/init.ld

CLEAN+=init.elf $(LDSCRIPT).i $(OBJS)
init.elf: $(LDSCRIPT).i $(OBJS)
	$(EL) ld $@
	$(CC) $(CFLAGS) $(LDFLAGS) -T $(LDSCRIPT).i -o $@ $(OBJS) $(LIBS:%=-l%)

.PHONY: clean
clean:
	rm -rf $(CLEAN)
