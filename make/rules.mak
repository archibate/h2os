
.SUFFIXES:

%.asm.bin: %.asm
	$(EC) asmbin $<
	$(ASM) $(ASMFLAGS) -f bin -o $@ $<

%.asm.o: %.asm
	$(EC) asm $<
	$(ASM) $(ASMFLAGS) -f elf -o $@ $<

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
	$(ASM) $(ASMFLAGS) -M -o $*.o $< > $@

%.c.d: %.c
	$(EC) cdep $<
	$(CC) $(CFLAGS) -M -o $@ $<

%.d: %
	$(EC) dep $@
	$(CC) $(CFLAGS) -xc -D_GPCPP_=1 -M -MT $*.i -P -o $@ $<

%.elf: %.pe
	$(EL) objcopy $@
	$(OBJCOPY) -O elf32-i386 -S $< $@
