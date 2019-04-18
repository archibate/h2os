#include <l4/generic/softfault.h>
#include <l4/generic/mman.h>
#include <l4/generic/ipc.h>
#include <l4/generic/sched.h>
#include <l4/generic/thread.h>//
#include <l4/enum/thread-states.h>//
#include <l4/generic/softfault.h>
#include <l4/machine/mmu/pagefault.h>
#include <l4/generic/archmappage.h>
#include <l4/enum/sfipctypes.h>
#include <l4/misc/panic.h>
#include <l4/enum/errno.h>
#include <l4/misc/bug.h>

int softfault_mmap(struct fd_entry *fde, word_t vaddr, size_t size, unsigned int flags)
{
	//printk("sm: %p", current->mm);
	struct mregion *mreg = mm_new(current->mm, vaddr, vaddr + size);
	if (mreg == NULL)
		return -EFAULT;

	memcpy(&mreg->fde, fde, sizeof(struct fd_entry));
	return 0;
}

static void ipcbuf_write_faultinfo(void *ipcbuf, word_t off, unsigned int errcd)
{
	word_t *ib = ipcbuf;
	ib[0] = -SFIPC_FAULT;
	ib[1] = off;
	ib[2] = errcd;
}

static void ipcbuf_write_msyncinfo(void *ipcbuf, word_t off, size_t size)
{
	word_t *ib = ipcbuf;
	ib[0] = -SFIPC_MSYNC;
	ib[1] = off;
	ib[2] = size;
}

static int ipcbuf_read_faultres(void *ipcbuf, word_t *page)
{
	word_t *ib = ipcbuf;
	int succ = ib[0];
	*page = ib[1];
	return succ;
}

static int ipcbuf_read_msyncres(void *ipcbuf)
{
	word_t *ib = ipcbuf;
	int succ = ib[0];
	return succ;
}

void user_bad_fault(struct ktcb *proc)
{
	panic("user_bad_fault(%p, %d)", proc->fault_vaddr, proc->fault_errcd);
}

void softfault_callback(word_t vaddr, unsigned int errcd)
{
	printk("softfault_callback(%p, %d)", vaddr, errcd);
	//current->state = THREAD_SUSPEND;
	//thread_suspend(current);
	//return;

	current->fault_vaddr = vaddr;
	current->fault_errcd = errcd;

	//printk("sc: %p", current->mm);
	//printk("scsc %p", list_entry(current->mm->mregs.first, struct mregion, hlist)->start);
	//printk("!softfault_callback(%p, %d)", vaddr, errcd);
	struct mregion *mreg = mm_lookup(current->mm, vaddr);
	if (mreg == NULL)
		user_bad_fault(current);
	printk("!!!softfault_callback(%p, %d)", vaddr, errcd);

	word_t off = vaddr - mreg->start;
	ipcbuf_write_faultinfo(current->ipcbuf, off, errcd);

	current->sfipc_type = SFIPC_FAULT;
	endp_call(&mreg->fde, true, true, -SFIPC_FAULT);
	//printk("soncall: %p->%p", current, sched_get_curr());
}

static void softfault_onreply_fault(struct ktcb *target)
{
	printk("softfault_onreply_fault: %p->%p", current, target);

	word_t page;
	int succ = ipcbuf_read_faultres(current->ipcbuf, &page);
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

static void softfault_onreply_msync(struct ktcb *target)
{
	int succ = ipcbuf_read_msyncres(current->ipcbuf);
	target->context.eax = succ; //T: eax
}

void softfault_onreply(struct ktcb *target)
{
	unsigned int type = target->sfipc_type;
	target->sfipc_type = 0;
	switch (type) {
	case SFIPC_FAULT: return softfault_onreply_fault(target);
	case SFIPC_MSYNC: return softfault_onreply_msync(target);
	default: BUG();
	}
}

int softfault_msync(word_t vaddr, size_t size)
{
	struct mregion *mreg = mm_lookup(current->mm, vaddr);
	if (mreg == NULL)
		return -EFAULT;

	word_t off = vaddr - mreg->start;
	if (vaddr + size >= mreg->end)
		return -EFAULT;

	ipcbuf_write_msyncinfo(current->ipcbuf, off, size);

	current->sfipc_type = SFIPC_MSYNC;
	endp_call(&mreg->fde, true, true, -SFIPC_MSYNC);
}
