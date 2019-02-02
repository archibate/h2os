#include <libl4/cspace.h>
#include <libl4/invoke.h>
#include <l4/arguments.h>
#include <l4/services.h>

int l4CSpace_SetDestSlot(l4CPtr_t cptr, l4CPtr_t slotptr)
{
	l4Word_t msg[] =
	{
		[L4_Arg_Service] = L4_CSpace_SetDestSlot,
		[L4_CSpace_SetDestSlot_Arg_SlotCPtr] = slotptr,
	};
	return l4Invoke(cptr, &msg, sizeof(msg));
}
