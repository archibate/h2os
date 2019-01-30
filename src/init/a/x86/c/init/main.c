#include <libl4/service.h>
#include <l4/inicaps.h>
#include <l4/offsets.h>

void init_main(void)
{
	static char s[] = "Hello, seL4!\n";
	l4Send(L4_InitialConsoleCap, 0, s, sizeof(s) - 1);
	l4Send(L4_InitialDebugCap, L4_Debug_Puts, s, sizeof(s) - 1);
	l4Send(L4_InitialDebugCap, L4_Debug_Halt, 0, 0);
}
