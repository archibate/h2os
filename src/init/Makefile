include tools/general.mak

.PHONY: all
all: init.elf.strip

SRCS+=$(shell find $(SRCPATH) | grep '\.\(c\|asm\)$$')
OBJS=$(SRCS:%=%.o)
LIBS=l4 c

LDSCRIPT=a/$(ARCH)/init.ld

CLEAN+=init.elf $(LDSCRIPT).i $(OBJS)
init.elf: $(LDSCRIPT).i $(OBJS)
	$(EL) ld $@
	$(CC) $(CFLAGS) $(LDFLAGS) -T $(LDSCRIPT).i -o $@ $(OBJS) $(LIBS:%=-l%)

.PHONY: clean
clean:
	rm -rf $(CLEAN)
