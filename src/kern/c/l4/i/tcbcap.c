#include <l4/i/tcbcap.h>
#include <l4/a/pgdir.h>
#include <l4/a/utcb.h>
#include <l4/errors.h>
#include <memory.h>

int do_TCB_SetCap(tcb_t *tcb, word_t cidx, cap_t *cap)
{
	if (tcb->state != TCB_NullState)
		return -L4_EActived;
	if (cidx > L4_TCBCapsMax)
		return -L4_ECapIdx;
	cap_t *dest = &tcb->caps[cidx];
	if (!cap)
		return -L4_ECLookup;
	memcpy(dest, cap, sizeof(cap_t));
	switch (cidx)
	{
	case L4_TCBCap_Pgdir:
		Arch_InitPgdir(dest->c_objptr);
		break;
	case L4_TCBCap_UTCB:
		Arch_InitUTCB(dest->c_objptr);
		break;
	}
	return 0;
}

int do_TCB_GetCap(tcb_t *tcb, word_t cidx, cap_t *capDest)
{
	if (tcb->state != TCB_NullState)
		return -L4_EActived;
	if (cidx > L4_TCBCapsMax)
		return -L4_ECapIdx;
	cap_t *src = &tcb->caps[cidx];
	memcpy(capDest, src, sizeof(cap_t));
	return 0;
}
