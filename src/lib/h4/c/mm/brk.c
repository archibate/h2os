#include <h4/mm.h>

void *brk(void *p)
{
	void *b = sbrk(0);
	return b != (void*)-1 ? b : sbrk(p - b);
}
