#include <libl4/io.h>
#include <libl4/mm.h>
#include <libl4/thread.h>
#include <libl4/puts.h>
#include <libl4/debug.h>
#include <l4/captypes.h>
#include <l4/inicaps.h>

void init_main(void)
{
	l4Puts(L4_InitCapConsole, "0123456789abcdefghijklmnopqrstuvwxyz\n");
	l4Segment_AllocSlab(L4_InitCapSigma0, 1);
	l4Slab_Retype(L4_InitCapDestSlot0, L4_TCBCap);
	l4Slab_Allocate(L4_InitCapDestSlot0, 1);
	l4ThreadContext_t context;
	context[L4_Context_EDI] = 0xcafebabe;
	l4TCB_SetContext(L4_InitCapDestSlot0, &context);
	l4Debug_Puts("Hello, L4Debug!");
	l4Debug_Halt();
}
