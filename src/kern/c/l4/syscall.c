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
#include <l4/clookup.h>
//#include <k/a/dumpuser.h>

int _FASTCALL systemCall(cptr_t cptr, word_t *shortMsg)
{
	int res;
	schedEnter();
	cap_t *cap = capLookup(cptr);
	cap_t *capDest = capGetDestSlot();
	if (!cap) {
		panic("syscall: fail to lookup cap %#x", cptr);
		res = -L4_ECTarget;
		goto out;
	}

	extern const char *__ntNameTableOfEnum_L4_CapType[];
	//extern const char *__ntNameTableOfEnum_L4_InitCapPtr[];
	extern const char *__ntNameTableOfEnum_L4_ServiceNumber[];
	if (cptr != L4_InitCapExtra) dprintk("&%x<%s$%x@%x[%x:%x%%%x]>: %s(%x)[%.8s%.7s]",
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
	//dprintk("susca:%p", __builtin_return_address(0));
	return res;
}
