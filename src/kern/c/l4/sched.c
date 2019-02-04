#include <l4/sched.h>
#include <mmu/mmu.h>
#include <assert.h>

queue_t running;
tcb_t *currTcb;

void schedLeave(void)
{
	tcb_t *next = schedGetCurr();
	assert(next->pgdirPaddr);
	if (next != currTcb)
		mmu_setPgdirPaddr(next->pgdirPaddr);
}
