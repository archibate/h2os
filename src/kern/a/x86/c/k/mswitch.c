#include <k/a/mswitch.h>
#include <k/bootml.h>
#include <mmu/mmu.h>
#include <memory.h>
#include <assert.h>
#include <k/panic.h>

void init_pgdir(pa_t pd)
{
	const size_t len = PdeIndex(KernPhysEnd) * sizeof(pde_t);
	memcpy((void*)pd, (void*)KernPdAddr, len);
}

void switch_pgdir(pa_t oldPd, pa_t newPd)
{
	const size_t len = PdeIndex(KernVirtEnd) * sizeof(pde_t);
	const size_t off = PdeIndex(KernVirtBegin) * sizeof(pde_t);
	assert(oldPd);
	assert(newPd);
	assert(!PageOffset(oldPd));
	assert(!PgdirOffset(newPd));
	if (!*(int*)newPd)init_pgdir(newPd);//
	memcpy((void*)newPd + off, (void*)oldPd + off, len);
	mmu_setPgdirPaddr(newPd);
}
