DEFINES+=_KERNEL=1
DEFINES+=CONFIG_CONSOLE_CLEAR=1 \
	 CONFIG_DEBUG_LOG=1 \
	 CONFIG_DEBUG_OBJECT=1 \
	 CONFIG_DEBUG_TRACE=1 \
	 CONFIG_DUMP_MODULES=1 \
	 CONFIG_DUMP_LDMOD=1 \
	 #CONFIG_LOG_SCHED_TIMER=1 \
	 CONFIG_LOG_EXTRA_SWI=1
INCLUDES+=h/l4/stdafx.h
LIBS+=debug c
include $(ROOT)/tools/general.mak

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

LDSCRIPT=a/$(ARCH)/kernel.ld

CLEAN+=vmlinux.elf $(LDSCRIPT).i $(OBJS)
vmlinux.elf: $(LDSCRIPT).i $(OBJS)
	$(EL) ld $@
	$(CC) $(CFLAGS) $(LDFLAGS) -T $(LDSCRIPT).i -o $@ $(OBJS) $(LIBS:%=-l%)

ALL+=out/boot/vmlinux-l4
out/boot/vmlinux-l4: vmlinux.elf
	@mkdir -p $(@D)
	$(EL) strip $@
	$(STRIP) $(STRIPFLAGS) -o $@ $<

ALL+=out/include/l4
out/include/l4: $(ADIRS:%=%/h/l4)
	@mkdir -p $(@D)
	@rm -rf $@
	cp -r $^ $(@D)

c/sys/systab.gen.c: $(wildcard h/l4/api/*.h)
	$(EL) gen $@
	./update-api.sh

.PHONY: all
all: $(ALL)

.PHONY: clean
clean:
	rm -rf $(CLEAN)

.PHONY: distclean
distclean:
	rm -rf $(CLEAN) out
