#include <k/x86/idt.h>
#include <k/asm/iframe.h>
#include <k/printk.h>

void hwintr(struct intr_frame *iframe)
{
	int nr = iframe->ifr_intnr;
	printk("hwintr(%d/%#x)", nr, nr);
}
