include tools/general.mak

.PHONY: all
all: libl4.a

SRCS+=$(shell find $(SRCPATH) | grep '\.\(c\|asm\)$$')
OBJS=$(SRCS:%=%.o)

CLEAN+=libl4.a $(OBJS)
libl4.a: $(OBJS)
	$(EL) ar $@
	$(AR) cqs $@ $(OBJS)

.PHONY: clean
clean:
	rm -rf $(CLEAN)
