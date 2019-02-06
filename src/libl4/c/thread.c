#include <libl4/thread.h>
#include <libl4/errors.h>
#include <libl4/message.h>
#include <libl4/invoke.h>
#include <l4/arguments.h>
#include <l4/services.h>

int l4TCB_SetContext(l4CPtr_t cptr, l4ThreadContext_t const *context)
{
	l4Word_t header[] =
	{
		[L4_Arg_Service] = L4_TCB_SetContext,
	};
	l4MsgInit();
	if (l4MsgWrite(header, sizeof(header)))
		return -Libl4_Error;
	if (l4MsgWrite(context, sizeof(l4ThreadContext_t)))
		return -Libl4_Error;
	return l4MsgInvoke(cptr);
}

int l4TCB_SetPriority(l4CPtr_t cptr, l4Byte_t priority)
{
	l4Word_t msg[] =
	{
		[L4_Arg_Service] = L4_TCB_SetPriority,
		[L4_TCB_SetPriority_Arg_Priority] = priority,
	};
	return l4Invoke(cptr, &msg, sizeof(msg));
}

int l4TCB_SetPCSP(l4CPtr_t cptr, l4Word_t pc, l4CPtr_t sp)
{
	l4Word_t msg[] =
	{
		[L4_Arg_Service] = L4_TCB_SetPCSP,
		[L4_TCB_SetPCSP_Arg_PC] = pc,
		[L4_TCB_SetPCSP_Arg_SP] = sp,
	};
	return l4Invoke(cptr, &msg, sizeof(msg));
}

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

int l4TCB_GetExtraBuffer(l4CPtr_t cptr)
{
	l4Word_t msg[] =
	{
		[L4_Arg_Service] = L4_TCB_GetExtraBuffer,
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
