#include <l4/i/tcbctx.h>
#include <l4/captypes.h>
#include <l4/errors.h>
#include <l4/a/utcb.h>

int do_TCB_SetPCSP(tcb_t *tcb, word_t pc, word_t sp)
{
	if (tcb->t_utcb.ctype != L4_PageCap)
		return -L4_ERetype;
	utcb_t *utcb = (void*)tcb->t_utcb.c_page.objaddr;

	utcb->iframe[IFrame_PC] = pc;
	utcb->iframe[IFrame_SP] = sp;
	utcb->seframe[SEFrame_PC] = pc;
	utcb->seframe[SEFrame_SP] = sp;
	return 0;
}
