#include <k/inicaps.h>
#include <l4/consts.h>
#include <l4/errors.h>
#include <l4/captypes.h>
#include <l4/inicaps.h>
#include <k/kbase.h>

void init_mycaps(cap_t *cspace, tcb_t *tcb)
{
	cspace[L4_InitCapNull] = (cap_t)
	{
		.c_type = L4_NullCap,
	};
	cspace[L4_InitCapConsole] = (cap_t)
	{
		.c_type = L4_ConsoleCap,
	};
	cspace[L4_InitCapIOPort] = (cap_t)
	{
		.c_type = L4_IOPortCap,
		.c_base = 0x0,
		.c_limit = 0x1000,
	};
	cspace[L4_InitCapDebug] = (cap_t)
	{
		.c_type = L4_DebugCap,
	};
	cspace[L4_InitCapDestSlot0] = (cap_t)
	{
		.c_type = L4_NullCap,
	};
	cspace[L4_InitCapSigma0] = (cap_t)
	{
		.c_type = L4_SegmentCap,
		.c_base = KernSigma0Begin,
		.c_limit = KernSigma0End - KernSigma0Begin,
		.c_water = 0,
	};
	cspace[L4_InitCapTCB] = (cap_t)
	{
		.c_type = L4_TCBCap,
		.c_objptr = tcb,
		.c_water = 0,
	};
	cspace[L4_InitCapCSpace] = (cap_t)
	{
		.c_type = L4_CRefCap,
		.c_objptr = &tcb->t_cspace,
	};
	cspace[L4_InitCapExtra] = (cap_t)
	{
		.c_type = L4_BufferCap,
		.c_objptr = &tcb->extraBuf,
		//.c_base = &tcb->extraBuf,
		.c_limit = L4_MaxExtraWords * sizeof(word_t),
		.c_water = 0,
	};
}
