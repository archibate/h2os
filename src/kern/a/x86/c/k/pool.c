#include <k/pool.h>
#include <k/kbase.h>
#include <mmu/page.h>
#include <memory.h>
#include <assert.h>

static pa_t pa0 = KernPoolBegin, pa1 = KernPoolEnd;

void *calloc(size_t nmemb, size_t size)
{
	pa_t pa = RoundUp(size, pa0);
	pa0 = pa;
	pa0 += size * nmemb;
	assert(pa0 < pa1);
	void *p = (void*)pa;//P2V?
	memset(p, 0, size);
	return p;
}
