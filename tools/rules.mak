
.SUFFIXES:

%.asm.bin: %.asm
	$(EC) asmbin $<
	$(ASM) $(ASMFLAGS) -f bin -o $@ $<

%.asm.o: %.asm
	$(EC) asm $<
	$(ASM) $(ASMFLAGS) -f elf -o $@ $<
ifdef MINGW
ifdef MAKE_A_LIB
	$(OBJCOPY) -O pe-i386 $@
endif
endif

%.c.o: %.c
	$(EC) cc $<
	$(CC) $(CFLAGS) -c -o $@ $<

%.i: %
	$(EC) cpp $<
	$(CC) $(CFLAGS) -xc -D_GPCPP_=1 -E -P -o $@ $<

%.S.o: %.S
	$(EC) as $<
	$(CC) $(CFLAGS) -D__ASSEMBLER__=1 -D_GPCPP_=1 -c -o $@ $<

%.S.d: %.S
	$(EC) asdep $<
	$(CC) $(CFLAGS) -D__ASSEMBLER__=1 -D_GPCPP_=1 -M -o $@ $<

%.asm.d: %.asm
	$(EC) asmdep $<
	$(ASM) $(ASMFLAGS) -M -MT $<.o -o $*.o $< > $@

%.c.d: %.c
	$(EC) cdep $<
	$(CC) $(CFLAGS) -M -MT $<.o -o $@ $<

%.d: %
	$(EC) dep $@
	$(CC) $(CFLAGS) -xc -D_GPCPP_=1 -M -MT $*.i -P -o $@ $<

%.strip: %
	$(EL) strip $<
	$(STRIP) $(STRIPFLAGS) -o $@ $<
