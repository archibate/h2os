#include <l4/syscall.h>
#include <l4/capability.h>
#include <l4/invoke.h>
#include <k/printk.h>
#include <k/panic.h>
#include <assert.h>
#include <l4/errors.h>
#include <l4/sched.h>
#include <l4/captypes.h>
#include <l4/inicaps.h>

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
	int res;
	schedEnter();
	cap_t *cspace = uncref(&currTcb->cspace);
	assert(cspace->c_type == L4_CSpaceCap);
	cap_t *cap = uncref(csLookup(cspace, cptr));
	cap_t *capDest = uncref(csGetDestSlot(cspace));
	assert(capDest);
	if (!cap) {
		panic("syscall: fail to lookup cap %#x", cptr);
		res = -L4_ECapLookup;
		goto out;
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
	res = sysInvoke(cap, capDest, shortMsg, currTcb->extraBuf);

	extern const char *__ntNameTableOfEnum_L4_ErrorNumber[];
	if (res < 0) panic("sysInvoke returned %d (%s)",
			res, __ntNameTableOfEnum_L4_ErrorNumber[-res]);
	//else dprintk("returning %d", res);

out:
	schedLeave();
	return res;
}
