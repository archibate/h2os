#include <l4/boot/calloc.h>
#include <l4/system/kbase.h>
#include <l4/lib/roundtools.h>
#include <l4/misc/assert.h>
#include <memory.h>

static uintptr_t pa0 = KernPoolBegin, pa1 = KernPoolEnd;

void *calloc(size_t nmemb, size_t size)
{
	uintptr_t pa = RoundUp(size, pa0);
	pa0 = pa;
	pa0 += size * nmemb;
	assert(pa0 < pa1);
	void *p = (void*)pa;//P2V?
	memset(p, 0, size * nmemb);
	return p;
}
