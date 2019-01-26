#include <k/x86/idt.h>
#include <k/asm/iframe.h>
#include <k/printk.h>

void hwintr(struct intr_frame *iframe)
{
  printk("hwintr(%d/%#x)", iframe->ifr_intnr, iframe->ifr_intnr);
}
