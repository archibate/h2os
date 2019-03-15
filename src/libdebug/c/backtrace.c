#include <backtrace.h>
#include <printk.h>

void backtrace(void)
{
	struct link {
		void *bp;
		void *pc;
	} *bp;
	asm volatile ("mov %%ebp, %0" : "=r" (bp));

	printk("backtrace:");
	int i;
	for (i = 0; i < 4 && bp; i++) {
		printk("#%d bp=%p: pc=%p", i, bp, bp->pc);
		bp = bp->bp;
	}
}
