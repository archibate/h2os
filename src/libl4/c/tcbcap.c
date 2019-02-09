#include <libl4/tcbcap.h>
#include <libl4/invoke.h>
#include <l4/arguments.h>
#include <l4/services.h>

int l4TCB_SetCap(l4CPtr_t cptr, l4Word_t cidx, l4CPtr_t cap)
{
	l4Word_t msg[] =
	{
		[L4_Arg_Service] = L4_TCB_SetCap,
		[L4_TCB_SetCap_Arg_CapIdx] = cidx,
		[L4_TCB_SetCap_Arg_CPtr] = cap,
	};
	return l4Invoke(cptr, &msg, sizeof(msg));
}

int l4TCB_GetCap(l4CPtr_t cptr, l4Word_t cidx)
{
	l4Word_t msg[] =
	{
		[L4_Arg_Service] = L4_TCB_GetCap,
		[L4_TCB_GetCap_Arg_CapIdx] = cidx,
	};
	return l4Invoke(cptr, &msg, sizeof(msg));
}
