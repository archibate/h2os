#include <l4/boot/mytcb.h>
#include <l4/system/kbase.h>
#include <l4/boot/inicaps.h>
#include <l4/boot/pool.h>
#include <l4/misc/printk.h>
#include <l4/object/thread.h>
#include <l4/machine/mmu/mmu.h>

static tcb_t tcbpool[12];
static uint maxtid = 0;

void make_mytcb(void *utcb, void *pgdir)
{
	cap_t *cspace = calloc(L4_InitCapMax, sizeof(cap_t));
	tcb_t *tcb = &tcbpool[maxtid++];
	init_mycaps(cspace, tcb);
	tcb->t_cspace = (cap_t)
	{
		.ctype = L4_CSpaceCap,
		.c_cspace.objptr = cspace,
		.c_cspace.limit = L4_InitCapMax,
		.c_cspace.water = L4_InitCapDestSlot0,
	};
	tcb->t_pgdir = (cap_t)
	{
		.ctype = L4_PgdirCap,
		.c_pgdir.objaddr = (word_t)pgdir,
	};
	tcb->t_utcb = (cap_t)
	{
		.ctype = L4_PageCap,
		.c_page.objaddr = (word_t)utcb,
	};
	tcb->state = TCB_Running;
	schedActive(tcb);
}
