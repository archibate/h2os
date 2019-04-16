#include <l4/generic/softfault.h>
#include <l4/generic/mman.h>
#include <l4/generic/ipc.h>
#include <l4/generic/sched.h>
#include <l4/generic/softfault.h>
#include <l4/machine/mmu/pagefault.h>
#include <l4/generic/archmappage.h>
#include <l4/misc/panic.h>
#include <l4/misc/bug.h>

static void ipcbuf_write_faultinfo(void *ipcbuf, word_t off, unsigned int errcd)
{
	word_t *ib = ipcbuf;
	ib[0] = -1;
	ib[1] = off;
	ib[2] = errcd;
}

static void ipcbuf_read_faultres(void *ipcbuf, int *succ, word_t *page)
{
	word_t *ib = ipcbuf;
	*succ = ib[0];
	*page = ib[1];
}

void user_bad_fault(struct ktcb *proc)
{
	panic("user_bad_fault(%p, %d)", proc->fault_vaddr, proc->fault_errcd);
}

void softfault_callback(word_t vaddr, unsigned int errcd)
{
	printk("softfault_callback(%p, %d)", vaddr, errcd);

	current->fault_vaddr = vaddr;
	current->fault_errcd = errcd;

	struct mregion *mreg = mm_lookup(current->mm, vaddr);
	if (mreg == NULL)
		user_bad_fault(current);

	word_t off = vaddr - mreg->start;
	ipcbuf_write_faultinfo(current->ipcbuf, off, errcd);

	current->isfault = true;
	endp_call(&mreg->fde, true, true, 136);
	//printk("soncall: %p->%p", current, sched_get_curr());
}

void softfault_onreply(struct ktcb *target)
{
	printk("onreply: %p->%p", current, target);

	word_t page;
	int succ;
	ipcbuf_read_faultres(current->ipcbuf, &succ, &page);
	BUG_ON(!current->isfault);
	current->isfault = false;
	if (succ < 0) {
bad:		user_bad_fault(target);
		return;
	}
	int ret = arch_mm_map_page(target->mm, target->fault_vaddr, succ, page);
	if (ret < 0) {
		/*target->fault_addr = vaddr;
		//target->fault_errcd = PageFaultErrorCode_Protect;
		BUG();
		user_bad_fault(target);
		//softfault_callback(page, succ & (PageFaultErrorCodeMask));*/
		goto bad;
	}
	return;
}

