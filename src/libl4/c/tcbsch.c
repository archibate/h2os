#include <libl4/tcbsch.h>
#include <libl4/invoke.h>
#include <l4/arguments.h>
#include <l4/services.h>

int l4TCB_SetPriority(l4CPtr_t cptr, l4Byte_t priority)
{
	l4Word_t msg[] =
	{
		[L4_Arg_Service] = L4_TCB_SetPriority,
		[L4_TCB_SetPriority_Arg_Priority] = priority,
	};
	return l4Invoke(cptr, &msg, sizeof(msg));
}

int l4TCB_Active(l4CPtr_t cptr)
{
	l4Word_t msg[] =
	{
		[L4_Arg_Service] = L4_TCB_Active,
	};
	return l4Invoke(cptr, &msg, sizeof(msg));
}

int l4TCB_Suspend(l4CPtr_t cptr)
{
	l4Word_t msg[] =
	{
		[L4_Arg_Service] = L4_TCB_Suspend,
	};
	return l4Invoke(cptr, &msg, sizeof(msg));
}
