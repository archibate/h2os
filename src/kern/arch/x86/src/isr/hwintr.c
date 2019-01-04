#include <hydro/x86/idt.h>
#include <hydro/asm/iframe.h>
#include <hydro/printk.h>

void hwintr(struct intr_frame *iframe)
{
  printk("hwintr(%d/%#x)", iframe->ifr_intnr, iframe->ifr_intnr);
}
