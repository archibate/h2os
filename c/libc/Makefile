include $Dmake/general.mak

.PHONY: all
all: libc.a

SRCS+=$(shell $Dscripts/r-wildexts.sh c asm)
OBJS=$(SRCS:%=%.o)

CLEAN+=libc.a $(OBJS)
libc.a: $(OBJS)
	$(EL) ar $@
	$(AR) cqs $@ $(OBJS)

.PHONY: clean
clean:
	rm -rf $(CLEAN)
