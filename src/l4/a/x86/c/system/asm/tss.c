#include <l4/system/kstack.h>
#include <l4/system/asm/tss.h>
#include <l4/system/asm/gdt.h>
#include <l4/system/asm/iframe.h>
#include <l4/machine/asm/gate.h>
#include <l4/machine/asm/xdtr.h>
#include <stddef.h>

struct tss tss0;

void init_tss(void)
{
	tss0.ts_ss0 = SEG_KDATA;
	tss0.ts_esp0 = KernIStackTop;
	tss0.ts_iomb = offsetof(struct tss, ts_iomap);
	tss0.ts_iomap[0] = 0xff;

	gdt[SEG_TSS0 >> 3] = SEGD16(STS_T32A, (unsigned long) &tss0, sizeof(tss0), 0);
	gdt[SEG_TSS0 >> 3].sd_s = 0;

	set_tr(SEG_TSS0);
}
