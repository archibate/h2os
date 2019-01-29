include tools/general.mak

.PHONY: all
all: libc.a

SRCS+=$(shell find c | grep '\.c$$')
OBJS=$(SRCS:%=%.o)

CLEAN+=libc.a $(OBJS)
libc.a: $(OBJS)
	$(EL) ar $@
	$(AR) cqs $@ $(OBJS)

.PHONY: clean
clean:
	rm -rf $(CLEAN)
