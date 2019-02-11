#include <l4/system/asm/idt.h>
#include <l4/system/asm/gdt.h>
#include <l4/machine/asm/xdtr.h>

struct gatedesc idt[NIDT_ENTS];

void init_idt(void)
{
	extern ulong __intrents[NIDT_ENTS];
	int i;

	for (i = 0; i < NIDT_ENTS; i++)
	{
		int dpl = i >= INTR_SWI0 ? 3 : 0;
		SETGATE(idt[i], 0, SEG_KCODE, __intrents[i], dpl);
	}

	load_idt(&idt, sizeof(idt));
}
