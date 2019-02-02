#include <libl4/debug.h>
#include <libl4/invoke.h>
#include <l4/arguments.h>
#include <l4/services.h>
#include <libl4/captrs.h>
#include <libl4/puts.h>

int l4Debug_Halt(void)
{
	l4Word_t msg[] =
	{
		[L4_Arg_Service] = L4_Debug_Halt,
	};
	return l4Invoke(Libl4_CapDebug, &msg, sizeof(msg));
}

int l4Debug_Puts(const char *s)
{
	return l4Puts(Libl4_CapDebug, s);
}
