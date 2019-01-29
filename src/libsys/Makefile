include tools/general.mak

.PHONY: all
all: libsys.a

SRCS+=$(shell find a/$(ARCH)/c c | grep '\.\(c\|asm\)$$')
OBJS=$(SRCS:%=%.o)

CLEAN+=libsys.a $(OBJS)
libsys.a: $(OBJS)
	$(EL) ar $@
	$(AR) cqs $@ $(OBJS)

.PHONY: clean
clean:
	rm -rf $(CLEAN)
