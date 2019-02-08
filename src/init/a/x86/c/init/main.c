#include <libl4/io.h>
#include <libl4/mm.h>
#include <libl4/page.h>
#include <libl4/cspace.h>
#include <libl4/thread.h>
#include <libl4/puts.h>
#include <libl4/debug.h>
#include <libl4/captrs.h>
#include <l4/captypes.h>
#include <l4/tcbcaps.h>
#include <memory.h>

#define l4SetDestSlot(cptr) l4CSpace_SetDestSlot(Libl4_CapCSpace, cptr)

void init_main(void)
{
	l4Puts(Libl4_CapConsole, "0123456789abcdefghijklmnopqrstuvwxyz\n");

	l4SetDestSlot(Libl4_CapSlab0);
	l4Segment_AllocPage(Libl4_CapMemory, 5);
	l4Page_Retype(Libl4_CapSlab0, L4_SlabCap, L4_TCBCap);
	l4Page_Retype(Libl4_CapPgdir0, L4_PgdirCap, 0);
	l4Page_Retype(Libl4_CapPgtab0, L4_PgtabCap, 0);

	l4SetDestSlot(Libl4_CapTCB0);
	l4Slab_Allocate(Libl4_CapSlab0, 1);

#if 0//{{{
	l4SetDestSlot(Libl4_CapPgdir0);
	l4Slab_Allocate(Libl4_CapSlab1, 1);

	l4SetDestSlot(Libl4_CapPgtab0);
	l4Slab_Allocate(Libl4_CapSlab2, 1);

	l4SetDestSlot(Libl4_CapPage0);
	l4Slab_Allocate(Libl4_CapSlab3, 1);
#endif//}}}

	l4Pgdir_MapPgtab(Libl4_CapPgdir, Libl4_CapPgtab0, 0x20000000);
	l4Pgdir_MapPage(Libl4_CapPgdir, Libl4_CapPage0, 0x20000000);
	extern char testcode_start[], testcode_end[];
	memcpy((void*)0x20000000, testcode_start, testcode_end - testcode_start);
	l4Pgdir_UnmapPage(Libl4_CapPgdir, 0x20000000);
	l4Pgdir_UnmapPgtab(Libl4_CapPgdir, 0x20000000);

	l4Pgdir_MapPgtab(Libl4_CapPgdir0, Libl4_CapPgtab0, 0x20000000);
	l4Pgdir_MapPage(Libl4_CapPgdir0, Libl4_CapPage0, 0x20000000);

#if 1
	l4TCB_SetCap(Libl4_CapTCB0, L4_TCBCap_Pgdir, Libl4_CapPgdir0);
	l4TCB_SetCap(Libl4_CapTCB0, L4_TCBCap_CSpace, Libl4_CapCSpace);
	l4TCB_SetCap(Libl4_CapTCB0, L4_TCBCap_UTCB, Libl4_CapPage1);
	l4TCB_SetPCSP(Libl4_CapTCB0, 0x20000000, 0x20000ffc);
	l4TCB_Active(Libl4_CapTCB0);
#endif

	for (;;)
		asm volatile ("int $0x82");

	l4Debug_Puts("Hello, L4Debug!");
	static char buf[273];
	l4Read(Libl4_CapDebug, buf, sizeof(buf));
	l4Debug_Puts(buf);
	l4Debug_Halt();
}
