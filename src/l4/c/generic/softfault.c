#include <l4/generic/softfault.h>
#include <l4/machine/mmu/pagefault.h>
#include <l4/misc/panic.h>

void softfault_callback(word_t vaddr, unsigned int errcd)
{
	panic("softfault_callback(%p, %d)", vaddr, errcd);

	errcd;
}
