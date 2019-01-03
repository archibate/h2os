include $Dmake/general.mak

.PHONY: all
all: kernel.elf

SRCS+=$(shell $Dscripts/r-wildexts.sh c asm)
OBJS=$(SRCS:%=%.o)
LIBS=c

CLEAN+=kernel.elf kernel.ld.i $(OBJS)
kernel.elf: kernel.ld.i $(OBJS)
	$(EL) ld $@
	$(CC) $(CFLAGS) $(LDFLAGS) -T kernel.ld.i -o $@ $(OBJS) $(LIBS:%=-l%)

.PHONY: clean
clean:
	rm -rf $(CLEAN)
