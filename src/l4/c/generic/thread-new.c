#include <l4/generic/thread.h>
#include <l4/generic/allocpage.h>
#include <l4/generic/kcache.h>
#include <l4/misc/init.h>

static struct kcache kcache_ktcb;

THIS_INIT {
	kcache_init(&kcache_ktcb, sizeof(struct ktcb));
}

struct ktcb *thread_new(void)
{
	struct ktcb *tcb = kcache_new(&kcache_ktcb);
	struct pgdir *pgdir = (void*)alloc_page();
	pgdir_init(pgdir);
	__thread_init(tcb);
	tcb->pgdir = pgdir;
	tcb->ipcbuf = (void*)alloc_page();
	return tcb;
}
