APPS=$(shell ls c/*.c | xargs basename -s .c)
LIBS+=c4 h4 debug h4 l4 c#T:c
CRTBEGIN=0
#SRCPATH+=$(ROOT)/src/lib/c4/crt
include $(ROOT)/tools/general.mak

.PHONY: default
default: all

.PHONY: showinfo
showinfo:
	@echo CFLAGS=$(CFLAGS)
	@echo LDFLAGS=$(LDFLAGS)
	@echo DEFINES=$(DEFINES)
	@echo INCLUDES=$(INCLUDES)
	@echo SRCPATH=$(SRCPATH)
	@echo INCPATH=$(INCPATH)
	@echo LIBPATH=$(LIBPATH)
	@echo ADIRS=$(ADIRS)
	@echo ROOT=$(ROOT)
	@echo APPS=$(APPS)

LDSCRIPT=$(ROOT)/out/lib/user.ld

CLEAN+=$(APPS:%=%.elf) $(LDSCRIPT).i $(APPS:%=%.c.o)
%.elf: c/%.c.o $(LDSCRIPT).i
	$(EL) ld $@
	$(CC) $(CFLAGS) $(LDFLAGS) -T $(LDSCRIPT).i -o $@ $(CRTBEGIN:%=$(ROOT)/out/lib/crt%.o) $< $(CRTEND:%=$(ROOT)/out/lib/crt%.o) $(LIBS:%=-l%)

ALL+=$(APPS:%=out/bin/%)
out/bin/%: %.elf
	@mkdir -p $(@D)
	$(EL) strip $@
	$(STRIP) $(STRIPFLAGS) -o $@ $<

.PHONY: all
all: $(ALL)

.PHONY: clean
clean:
	rm -rf $(CLEAN)

.PHONY: distclean
distclean:
	rm -rf $(CLEAN) out

#.PRECIOUS: c/%.c.o
