#include <c4/liballoc.h>

void __pre_init(void)
{
	/*extern void __libh4_init(void);
	__libh4_init();
	for (;;);*/

	static char heap[4096*4];
	liballoc_set_memory(&heap, sizeof(heap));
}
