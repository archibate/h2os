#include <libl4/io.h>
#include <libl4/mm.h>
#include <libl4/puts.h>
#include <libl4/debug.h>
#include <l4/captypes.h>
#include <l4/inicaps.h>

void init_main(void)
{
	l4Puts(L4_InitCapConsole, 0, "Hello, Console!\n");
	l4Allocate(L4_InitCapSigma0, 1, L4_TestObjCap);
	l4Puts(L4_InitCapDestSlot0, 0, "Hello, CapDestSlot0!");
	l4Debug_Puts("Hello, Debugger");
	l4Debug_Halt();
}
