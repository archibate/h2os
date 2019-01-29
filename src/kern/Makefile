include tools/general.mak

.PHONY: all
all: kernel.elf.strip

SRCS+=$(shell find c a/$(ARCH)/c | grep '\.\(c\|asm\)$$')
OBJS=$(SRCS:%=%.o)
LIBS=c

LDSCRIPT=a/$(ARCH)/kernel.ld

CLEAN+=kernel.elf $(LDSCRIPT).i $(OBJS)
kernel.elf: $(LDSCRIPT).i $(OBJS)
	$(EL) ld $@
	$(CC) $(CFLAGS) $(LDFLAGS) -T $(LDSCRIPT).i -o $@ $(OBJS) $(LIBS:%=-l%)

c/init/initimg.asm.o: ../init/init.elf.strip

.PHONY: clean
clean:
	rm -rf $(CLEAN)
