#include <l4/generic/utcb.h>
#include <l4/system/asm/gdt.h>
#include <l4/machine/asm/eflags.h>
#include <memory.h>

void utcb_init(struct utcb *utcb)
{
	memset(utcb, 0, sizeof(*utcb));

	word_t cs = SEG_UCODE;
	word_t ds = SEG_UDATA;

	utcb->iframe.eflags = FL_1F | FL_IF | FL_IOPL(3);
	utcb->iframe.cs = cs;
	utcb->iframe.ss = ds;
	utcb->iframe.ds = ds;
	utcb->iframe.es = ds;
	utcb->iframe.fs = ds;
	utcb->iframe.gs = ds;

	extern void _NORETURN utcb_exiter(void);
	utcb->exiter = utcb_exiter;
}
