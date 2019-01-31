#include <l4/memory.h>
#include <l4/a/mmapwin.h>
#include <mmu/page.h>
#include <numtools.h>
#include <memory.h>

size_t mwrite(mem_t *mem, word_t addr, byte_t const *data, size_t length)
{
	size_t len;
	void *page;

	do {
		page = mmapwin(mem, PageDown(addr), 1);
		if (!page)
			break;
		len = MAX(length, PageSize - PageOffset(addr));
		memcpy(page + PageOffset(addr), data, len);
		length -= len;
	} while (length != 0);
	return length;
}

size_t mread(mem_t *mem, word_t addr, byte_t *data, size_t length)
{
	size_t len;
	void *page;

	do {
		page = mmapwin(mem, PageDown(addr), 0);
		if (!page)
			break;
		len = MAX(length, PageSize - PageOffset(addr));
		memcpy(data, page + PageOffset(addr), len);
		length -= len;
	} while (length != 0);
	return length;
}
