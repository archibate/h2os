#include <libl4/io.h>
#include <libl4/mm.h>
#include <libl4/cspace.h>
#include <libl4/thread.h>
#include <libl4/puts.h>
#include <libl4/debug.h>
#include <libl4/captrs.h>
#include <l4/captypes.h>

void init_main(void)
{
	l4Puts(Libl4_CapConsole, "0123456789abcdefghijklmnopqrstuvwxyz\n");
	l4CSpace_SetDestSlot(Libl4_CapCSpace, Libl4_CapSlab0);
	l4Segment_AllocSlab(Libl4_CapMemory, 1);
	l4Slab_Retype(Libl4_CapSlab0, L4_TCBCap);
	l4CSpace_SetDestSlot(Libl4_CapCSpace, Libl4_CapTCB0);
	l4Slab_Allocate(Libl4_CapSlab0, 1);
	l4ThreadContext_t context;
	context[L4_Context_EDI] = 0xcafebabe;
	l4TCB_SetContext(Libl4_CapTCB0, &context);
	l4Debug_Puts("Hello, L4Debug!");
	static char buf[273];
	l4Read(Libl4_CapDebug, buf, sizeof(buf));
	l4Debug_Puts(buf);
	l4Debug_Halt();
}
