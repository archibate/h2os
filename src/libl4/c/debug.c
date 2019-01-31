#include <libl4/debug.h>
#include <libl4/puts.h>
#include <libl4/api.h>
#include <l4/services.h>
#include <l4/inicaps.h>

int l4Debug_Halt(void)
{
	return l4Send(L4_InitCapDebug, L4_Debug_Halt, 0, 0, 0);
}

int l4Debug_Puts(const char *s)
{
	return l4Puts(L4_InitCapDebug, 0, s);
}
