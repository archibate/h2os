#include <l4/generic/allocpage.h>
#include <l4/boot/calloc.h>

void *alloc_page(void)
{
	return calloc(1, PageSize);
}
