#include <l4/clookup.h>
#include <l4/cspace.h>
#include <l4/sched.h>
#include <k/printk.h>

cap_t *capLookup(cptr_t cptr)
{
	return uncref(csLookup(&currTcb->t_cspace.c_cspace, cptr));
}

cap_t *capGetDestSlot(void)
{
	return csGetDestSlot(&currTcb->t_cspace.c_cspace);
}
