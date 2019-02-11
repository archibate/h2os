#include <l4/generic/utcb.h>
#include <l4/system/asm/gdt.h>
#include <memory.h>

void utcb_init(struct utcb *utcb)
{
	memset(utcb, 0, sizeof(*utcb));

	word_t cs = SEG_UCODE;
	word_t ds = SEG_UDATA;

	utcb->iframe.cs = cs;
	utcb->iframe.ds = ds;
	utcb->iframe.es = ds;
	utcb->iframe.fs = ds;
	utcb->iframe.gs = ds;

	utcb->seframe.ds = ds;
	utcb->seframe.es = ds;
}
