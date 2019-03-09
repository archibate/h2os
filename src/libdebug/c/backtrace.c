#include <backtrace.h>
#include <printk.h>

void backtrace(void)
{
	struct link {
		void *pc;
		void *bp;
	} *bp;
	asm volatile ("mov %%ebp, %0" : "=r" (bp));

	printk("backtrace:");
	int i;
	for (i = 0; i < 2 && bp; i++) {
		printk("#%d %p: %p", i, bp, bp->pc);
		bp = bp->bp;
	}
}
