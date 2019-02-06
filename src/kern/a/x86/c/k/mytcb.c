#include <k/printk.h>
#include <k/mytcb.h>
#include <k/inicaps.h>
#include <l4/inicaps.h>
#include <l4/captypes.h>
#include <l4/thread.h>
#include <l4/sched.h>
#include <mmu/mmu.h>
#include <k/pool.h>

static tcb_t tcbpool[12];
static uint maxtid = 0;

void setup_mytcb(void)
{
	cap_t *cspace = calloc(L4_InitCapMax, sizeof(cap_t));
	tcb_t *tcb = &tcbpool[maxtid++];
	init_mycaps(cspace, tcb);
	tcb->t_cspace = (cap_t)
	{
		.c_type = L4_CSpaceCap,
		.c_objptr = cspace,
		.c_limit = L4_InitCapMax,
		.c_water = L4_InitCapDestSlot0,
	};
	tcb->t_pgdir = (cap_t)
	{
		.c_type = L4_PgdirCap,
		.c_objaddr = mmu_getPgdirPaddr(),
	};
	schedSetActive(tcb);
}