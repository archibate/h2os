#include <h4/sys/types.h>
#include <h4/sys/ipc.h>
#include <h4/servers.h>
#include <h4/file/sysnr.h>
#include <memory.h>
#include <errno.h>
#include <stddef.h>
#include <list.h>
#include <bug.h>
#include <panic.h>
#include <printk.h>
#include <c4/liballoc.h>
#include <malloc.h>
#include <numtools.h>
#include <l4/machine/mmu/page.h>

/////////////////////////////////

struct mpage_table
{
	struct hlist_head head;
	struct hello_file *fp;
	off_t base;
	size_t size;
	unsigned int flags;
};

struct mpage_entry
{
	struct hlist_node hlist;
	void *page;
	unsigned int pgnr;
};

void mpt_new_mpage(struct mpage_table *mpt, unsigned int pgnr, void *page)
{
	struct mpage_entry *mpe;
	/*hlist_for_each_entry2(mpe, &mpt->head, hlist) {
		if (mpe->pgnr == pgnr) {
			//free(mpe->page);
			mpe->page = page;
			return;
		}
	}*/
	mpe = zalloc(sizeof(struct mpage_entry));
	mpe->page = page;
	mpe->pgnr = pgnr;
	hlist_add_head(&mpe->hlist, &mpt->head);
	return;
}

void *mpt_lookup_mpage(struct mpage_table *mpt, unsigned int pgnr)
{
	struct mpage_entry *mpe;
	hlist_for_each_entry2(mpe, &mpt->head, hlist) {
		if (mpe->pgnr == pgnr) {
			BUG_ON(mpe->page == NULL);
			return mpe->page;
		}
	}
	return NULL;
}

/////////////////////////////////

struct hello_file
{
	void *data;
	size_t size;
	struct mpage_table mpt;
};

ssize_t hello_pread(struct hello_file *fp, void *buf, size_t len, off_t off)
{
	CLMAX(len, fp->size - off);
	memcpy(buf, fp->data + off, len);
	return len;
}

ssize_t hello_pwrite(struct hello_file *fp, const void *buf, size_t len, off_t off)
{
	CLMAX(len, fp->size - off);
	memcpy(fp->data + off, buf, len);
	return len;
}

off_t hello_lseek(struct hello_file *fp, off_t now_off, off_t off, int whence)
{
	switch (whence) {
	case 0: break;
	case 1: off += now_off; break;
	case 2: off = fp->size + off; break;
	default: return -ENOTSUP;
	};
	CLMIN(off, 0);
	CLMAX(off, fp->size);
	return off;
}

struct hello_file *hello_new_open(unsigned int flags)
{
	/*struct hello_file *fp = malloc(sizeof(struct hello_file));
	fp->id = conn;
	BUG_ON(NULL != idmap_add(&ftab, fp));*/
	struct hello_file *fp = zalloc(sizeof(struct hello_file));
	static char data[] = "Hello, World!\n";
	fp->data = &data;
	fp->size = sizeof(data)-1;
	BUG_ON(fp == NULL);
	return fp;
}

void hello_close(struct hello_file *fp)
{
	free(fp);
}

int hello_mpt_msync(struct mpage_table *mpt, size_t off, size_t size)
{
	void *page = mpt_lookup_mpage(mpt, off / PageSize);
	BUG_ON(page == NULL);
	hello_pwrite(mpt->fp, page + off % PageSize, size, mpt->base + off);
	return 0;
}

int hello_mpt_fault(struct mpage_table *mpt, size_t off, int errcd, void **ppage)
{
	void *page = amalloc(PageSize, PageSize);
	BUG_ON(mpt_lookup_mpage(mpt, off / PageSize) != NULL);
	mpt_new_mpage(mpt, off / PageSize, page);
	BUG_ON((uintptr_t)page & PageLomask);
	hello_pread(mpt->fp, page, PageSize, mpt->base + off);
	*ppage = page;
	return 0;
}

struct mpage_table *hello_mpt_open(struct hello_file *fp, off_t base, size_t size, unsigned int flags)
{
	struct mpage_table *mpt = zalloc(sizeof(struct mpage_table));

	mpt->flags = flags;
	mpt->size = size;
	mpt->base = base;
	mpt->fp = fp;
	return mpt;
}

void hello_serve_ipc(void *p)
{
	struct mpage_table *mpt = p;
	struct hello_file *fp = p;

	int nr = ipc_getw();

	//printk("nr=%d", nr == _FILE_mmap);
	if ((ipc_getoffset() != -1) != (nr >= 0 || nr == _FILE_mmap))
		goto notsup;

	switch (nr) {

	case _FILE_msync:
	{
		size_t inoff = ipc_getw();
		size_t size = ipc_getw();
		printk("hello_msync(%d, %d)", inoff, size);
		int succ = hello_mpt_msync(mpt, inoff, size);
		ipc_rewindw(succ);
	} break;

	case _FILE_fault:
	{
		size_t inoff = ipc_getw();
		int errcd = ipc_getw();
		printk("hello_fault(%d, %d)", inoff, errcd);
		void *page = NULL;
		int succ = hello_mpt_fault(mpt, inoff, errcd, &page);
		ipc_rewindw(succ);
		ipc_putw((uintptr_t)page);
	} break;

	case _FILE_mmap:
	{
		off_t base = ipc_getoffset();
		size_t size = ipc_getw();
		unsigned int flags = ipc_getw();
		printk("hello_mmap(%d)", base);
		mpt = hello_mpt_open(fp, base, size, flags);
		if (mpt != NULL) {
			ipc_setbadge((uintptr_t)mpt);
			ipc_setoffset(-1);
			ipc_rewindw(0);
		} else {
			ipc_rewindw(errno);
		}
	} break;

	case _FILE_pread:
	{
		size_t len = ipc_getw();
		off_t off = ipc_getw();
		//printk("hello_pread(%d, %d)", len, off);
		ipc_seek_setw(1);
		void *buf = ipc_getbuf(&len);
		ssize_t ret = hello_pread(fp, buf, len, off);
		ipc_rewindw(ret);
	} break;

	case _FILE_pwrite:
	{
		size_t len = ipc_getw();
		off_t off = ipc_getw();
		//printk("hello_pwrite(%d, %d)", len, off);
		const void *buf = ipc_getbuf(&len);
		ssize_t ret = hello_pwrite(fp, buf, len, off);
		ipc_rewindw(ret);
	} break;

	case _FILE_read:
	{
		size_t len = ipc_getw();
		//printk("hello_read(%d)", len);
		ipc_seek_setw(1);
		void *buf = ipc_getbuf(&len);
		off_t off = ipc_getoffset();
		ssize_t ret = hello_pread(fp, buf, len, off);
		if (ret > 0)
			off += ret;
		ipc_setoffset(off);
		ipc_rewindw(ret);
	} break;

	case _FILE_write:
	{
		size_t len = ipc_getw();
		//printk("hello_write(%d)", len);
		const void *buf = ipc_getbuf(&len);
		off_t off = ipc_getoffset();
		ssize_t ret = hello_pwrite(fp, buf, len, off);
		if (ret > 0)
			off += ret;
		ipc_setoffset(off);
		ipc_rewindw(ret);
	} break;

	case _FILE_lseek:
	{
		off_t off = ipc_getw();
		int whence = ipc_getw();
		off_t now_off = ipc_getoffset();
		off_t ret = hello_lseek(fp, now_off, off, whence);
		if (ret >= 0)
			ipc_setoffset(ret);
		ipc_rewindw(ret);
	} break;

	default:
notsup:		ipc_rewindw(-ENOTSUP);
	}
	ipc_reply();
}

const int libh4_serve_id = SVID_HELLO;

int main(void)
{
	static char buffer[4096*4];
	liballoc_set_memory(buffer, sizeof(buffer));

	while (1) {
		ipc_recv();
		void *p = (void*)ipc_getbadge();
		if (ipc_isclose()) {
			BUG();
			/*if (ipc_getoffset() == -1) {
				hello_close(p);
			} else {
				hello_close(p);
			}*/
		} else {
			if (p == NULL) {
				unsigned int flags = 0;//TOD
				p = hello_new_open(flags);
				//printk("fp=%p", p);
				ipc_setbadge((uintptr_t)p);
			}
			hello_serve_ipc(p);
		}
	}
}
