#include <l4/syscall.h>
#include <l4/capability.h>
#include <l4/invoke.h>
#include <k/printk.h>
#include <k/panic.h>
#include <assert.h>

#include <l4/consts.h>
#include <l4/errors.h>
#include <l4/thread.h>
tcb_t tcb0, *currTcb = &tcb0;
#include <k/kbase.h>
#include <l4/captypes.h>
#include <l4/inicaps.h>
void setup_mycaps(void)
{
	static cap_t cspace[L4_InitCapMax];
	tcb0.cspace = (cap_t)
	{
		.c_type = L4_CSpaceCap,
		.c_objptr = cspace,
		.c_limit = L4_InitCapMax,
		.c_water = L4_InitCapDestSlot0,
	};
	cspace[L4_InitCapCSpace] = (cap_t)
	{
		.c_type = L4_CRefCap,
		.c_objptr = &tcb0.cspace,
	};
	cspace[L4_InitCapNull] = (cap_t)
	{
		.c_type = L4_NullCap,
	};
	cspace[L4_InitCapConsole] = (cap_t)
	{
		.c_type = L4_ConsoleCap,
	};
	cspace[L4_InitCapIOPort] = (cap_t)
	{
		.c_type = L4_IOPortCap,
		.c_base = 0x0,
		.c_limit = 0x1000,
	};
	cspace[L4_InitCapDebug] = (cap_t)
	{
		.c_type = L4_DebugCap,
	};
	cspace[L4_InitCapTCB] = (cap_t)
	{
		.c_type = L4_TCBCap,
		.c_objptr = &tcb0,
		.c_water = 0,
	};
	cspace[L4_InitCapExtra] = (cap_t)
	{
		.c_type = L4_BufferCap,
		.c_objptr = &tcb0.extraBuf,
		//.c_base = &tcb0.extraBuf,
		.c_limit = L4_MaxExtraWords,
		.c_water = 0,
	};
	cspace[L4_InitCapSigma0] = (cap_t)
	{
		.c_type = L4_SegmentCap,
		.c_base = KernSigma0Begin,
		.c_limit = KernSigma0End - KernSigma0Begin,
		.c_water = 0,
	};
	cspace[L4_InitCapDestSlot0] = (cap_t)
	{
		.c_type = L4_NullCap,
	};
}

cap_t *csLookup(cap_t *cs, cptr_t cptr)
{
	if (cptr > cs->c_limit)
		return 0;
	return &((cap_t*)cs->c_objptr)[cptr];
}

cap_t *csGetDestSlot(cap_t *cs)
{
	assert(cs->c_water < cs->c_limit);
	return &((cap_t*)cs->c_objptr)[cs->c_water];
}

cap_t *uncref(cap_t *cap)
{
	if (!cap)
		return cap;
	if (cap->c_type == L4_CRefCap)
		cap = cap->c_objptr;
	assert(cap->c_type != L4_CRefCap);
	return cap;
}

int _FASTCALL systemCall(cptr_t cptr, word_t *shortMsg)
{
	cap_t *cspace = uncref(&currTcb->cspace);
	assert(cspace->c_type == L4_CSpaceCap);
	cap_t *cap = uncref(csLookup(cspace, cptr));
	cap_t *capDest = uncref(csGetDestSlot(cspace));
	assert(capDest);
	if (!cap) {
		panic("syscall: fail to lookup cap %#x", cptr);
		return -L4_ECapLookup;
	}

	extern const char *__ntNameTableOfEnum_L4_CapType[];
	//extern const char *__ntNameTableOfEnum_L4_InitCapPtr[];
	extern const char *__ntNameTableOfEnum_L4_ServiceNumber[];
	if (cptr != L4_InitCapExtra) dprintk("&%x<%s$%x@%x[%x:%x%%%x]>: %s(%x)[%.8s%.8s]",
			cptr,//10+__ntNameTableOfEnum_L4_InitCapPtr[cptr],
			3+__ntNameTableOfEnum_L4_CapType[cap->c_type], cap->c_retype,
			cap->c_objptr, cap->c_base, cap->c_limit, cap->c_water,
			3+__ntNameTableOfEnum_L4_ServiceNumber[shortMsg[0]],
			shortMsg[1],shortMsg+2,shortMsg[2]&0xff?currTcb->extraBuf:(void*)"");
	int res = sysInvoke(cap, capDest, shortMsg, currTcb->extraBuf);

	extern const char *__ntNameTableOfEnum_L4_ErrorNumber[];
	if (res < 0) panic("sysInvoke returned %d (%s)",
			res, __ntNameTableOfEnum_L4_ErrorNumber[-res]);
	//else dprintk("returning %d", res);

	return res;
}
