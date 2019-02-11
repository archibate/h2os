#include <k/inicaps.h>
#include <l4/consts.h>
#include <l4/errors.h>
#include <l4/captypes.h>
#include <l4/inicaps.h>
#include <mmu/page.h>
#include <k/kbase.h>

void init_mycaps(cap_t *cspace, tcb_t *tcb)
{
	cspace[L4_InitCapNull] = (cap_t)
	{
		.ctype = L4_NullCap,
	};
	cspace[L4_InitCapConsole] = (cap_t)
	{
		.ctype = L4_ConsoleCap,
	};
	cspace[L4_InitCapIOPort] = (cap_t)
	{
		.ctype = L4_IOPortCap,
		.c_ioseg.base = 0x0,
		.c_ioseg.limit = 0x1000,
	};
	cspace[L4_InitCapDebug] = (cap_t)
	{
		.ctype = L4_DebugCap,
	};
	cspace[L4_InitCapDestSlot0] = (cap_t)
	{
		.ctype = L4_NullCap,
	};
	cspace[L4_InitCapSigma0] = (cap_t)
	{
		.ctype = L4_SegmentCap,
		.c_segment.base = PageNum(KernSigma0Begin),
		.c_segment.limit = PageNum(KernSigma0End - KernSigma0Begin),
		.c_segment.water = 0,
	};
#if 0
	cspace[L4_InitCapVRAM] = (cap_t)
	{
		.ctype = L4_SegmentCap,
		.c_segment.base = PageNum(KernVRAMBegin),
		.c_segment.limit = PageNum(KernVRAMEnd - KernVRAMBegin),
		.c_segment.water = 0,
	};
#endif
	cspace[L4_InitCapTCB] = (cap_t)
	{
		.ctype = L4_TCBCap,
		.c_objptr = tcb,
	};
	cspace[L4_InitCapCSpace] = (cap_t)
	{
		.ctype = L4_CRefCap,
		.c_objptr = &tcb->t_cspace,
	};
	cspace[L4_InitCapPgdir] = (cap_t)
	{
		.ctype = L4_CRefCap,
		.c_objptr = &tcb->t_pgdir,
	};
	cspace[L4_InitCapExtra] = (cap_t)
	{
		.ctype = L4_BufferCap,
		.c_buffer.objptr = tcb->extraBuf,
		.c_buffer.limit = L4_MaxExtraWords * sizeof(word_t),
		.c_buffer.water = 0,
	};
}
