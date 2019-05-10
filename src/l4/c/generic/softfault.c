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

static void ipcbuf_write_faultinfo(void *ipcbuf, word_t off, unsigned int errcd)
{
	word_t *ib = ipcbuf;
	ib[0] = -SFIPC_FAULT;
	ib[1] = off;
	ib[2] = errcd;
}

static void ipcbuf_write_munmapinfo(void *ipcbuf, word_t off, size_t size)
{
	word_t *ib = ipcbuf;
	ib[0] = -SFIPC_MUNMAP;
	ib[1] = off;
	ib[2] = size;
}

static void ipcbuf_write_msyncinfo(void *ipcbuf, word_t off, size_t size)
{
	word_t *ib = ipcbuf;
	ib[0] = -SFIPC_MSYNC;
	ib[1] = off;
	ib[2] = size;
}

static void ipcbuf_write_mmapinfo(void *ipcbuf, size_t size, unsigned int flags)
{
	word_t *ib = ipcbuf;
	ib[0] = -SFIPC_MMAP;
	ib[1] = size;
	ib[2] = flags;
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

static int ipcbuf_read_munmapres(void *ipcbuf)
{
	word_t *ib = ipcbuf;
	int succ = ib[0];
	return succ;
}

static int ipcbuf_read_mmapres(void *ipcbuf)
{
	word_t *ib = ipcbuf;
	int succ = ib[0];
	return succ;
}

#define get_mmcmm(mmc) ((mmc) == -1 ? current->replying->mm : current->mm)

int softfault_mmap(l4id_t mmc, struct fd_entry *fde, word_t vaddr, size_t size, unsigned int flags, unsigned int prot)
{
	//printk("sm: %p", current->mm);
	struct mregion *mreg = mm_new(current->mm, vaddr, vaddr + size, prot);
	if (mreg == NULL)
		return -EFAULT;

	memcpy(&mreg->fde, fde, sizeof(struct fd_entry));

	ipcbuf_write_mmapinfo(current->ipcbuf, size, flags);

	//printk("!!!!!callsfipc_Mmap");
	current->sfipc_type = SFIPC_MMAP;
	current->sfipc_wilmreg = mreg;
	current->sfipc_mm = get_mmcmm(mmc);

	endp_call(&mreg->fde, true, true, -SFIPC_MMAP);
	return 0;
}

void user_bad_fault(struct ktcb *proc)
{
	panic("user_bad_fault(%p, %d)", proc->fault_vaddr, proc->fault_errcd);
}

void softfault_callback(l4id_t mmc, word_t vaddr, unsigned int errcd)
{
	//printk("softfault_callback(%p, %d)", vaddr, errcd);
	//current->state = THREAD_SUSPEND;
	//thread_suspend(current);
	//return;

	current->fault_vaddr = vaddr;
	current->fault_errcd = errcd;
	current->sfipc_mm = get_mmcmm(mmc);

	//printk("sc: %p", current->mm);
	//printk("scsc %p", list_entry(current->mm->mregs.first, struct mregion, hlist)->start);
	//printk("!softfault_callback(%p, %d)", vaddr, errcd);
	struct mregion *mreg = mm_lookup(current->mm, vaddr);
	if (mreg == NULL)
		user_bad_fault(current);
	//printk("scren!!");

	word_t off = vaddr - mreg->start;
	ipcbuf_write_faultinfo(current->ipcbuf, off, errcd);

	current->sfipc_type = SFIPC_FAULT;
	endp_call(&mreg->fde, true, true, -SFIPC_FAULT);
	//printk("soncall: %p->%p", current, sched_get_curr());
}

static void softfault_onreply_fault(struct ktcb *target)
{
	//printk("softfault_onreply_fault: %p->%p", current, target);

	word_t page;
	int succ = ipcbuf_read_faultres(current->ipcbuf, &page);
	//printk("succ=%d, page=%p, page[0]=%#x", succ, page, ((char*)page)[0]);
	if (succ < 0) {
bad:		user_bad_fault(target);
		return;
	}
	BUG_ON(target->sfipc_mm == NULL);
	int ret = arch_mm_map_page(target->sfipc_mm, target->fault_vaddr, succ, page);
	target->sfipc_mm = NULL;
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
	target->sfipc_mm = NULL;
	target->context.eax = succ; //T: eax
}

static void softfault_onreply_munmap(struct ktcb *target)
{
	int succ = ipcbuf_read_munmapres(current->ipcbuf);
	if (succ >= 0) {
		mm_del(target->sfipc_wilmreg);
	}
	target->sfipc_mm = NULL;
	target->sfipc_wilmreg = NULL;
}

static void softfault_onreply_mmap(struct ktcb *target)
{
	int succ = ipcbuf_read_mmapres(current->ipcbuf);
	target->context.eax = succ; //T: eax
	if (succ < 0) {
		//TOD: free that mreg
		mm_del(target->sfipc_wilmreg);
	}
	target->sfipc_mm = NULL;
	target->sfipc_wilmreg = NULL;
}

void softfault_onreply(struct ktcb *target)
{
	unsigned int type = target->sfipc_type;
	target->sfipc_type = 0;
	switch (type) {
	case SFIPC_FAULT:  return softfault_onreply_fault(target);
	case SFIPC_MSYNC:  return softfault_onreply_msync(target);
	case SFIPC_MMAP:   return softfault_onreply_mmap(target);
	case SFIPC_MUNMAP: return softfault_onreply_munmap(target);
	default: BUG();
	}
}

int softfault_msync(l4id_t mmc, word_t vaddr, size_t size)
{
	struct mregion *mreg = mm_lookup(current->mm, vaddr);
	if (mreg == NULL)
		return -EFAULT;

	word_t off = vaddr - mreg->start;
	if (vaddr + size >= mreg->end)
		return -EFAULT;

	ipcbuf_write_msyncinfo(current->ipcbuf, off, size);

	current->sfipc_type = SFIPC_MSYNC;
	current->sfipc_mm = get_mmcmm(mmc);
	endp_call(&mreg->fde, true, true, -SFIPC_MSYNC);
	return 0;
}

int softfault_munmap(l4id_t mmc, word_t vaddr, size_t size)
{
	struct mregion *mreg = mm_lookup(current->mm, vaddr);
	if (mreg == NULL)
		return -EFAULT;

	word_t off = vaddr - mreg->start;
	if (vaddr + size >= mreg->end)
		return -EFAULT;

	ipcbuf_write_munmapinfo(current->ipcbuf, off, size);

	current->sfipc_wilmreg = mreg;
	current->sfipc_type = SFIPC_MUNMAP;
	current->sfipc_mm = get_mmcmm(mmc);
	endp_call(&mreg->fde, true, true, -SFIPC_MUNMAP);

	return 0;
}
