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

#if 0
void *fuck_malloc(size_t size)
{
	printk("zero: fuck_malloc(%d)", size);
	return malloc(size);
}
#define malloc fuck_malloc
#endif

#define ZERO_FRGSIZ 256

/////////////////////////////////

struct mpage_table
{
	struct hlist_head head;
	struct zero_file *fp;
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

struct zero_file
{
	void *data;
	size_t size;
	struct mpage_table mpt;
};

static void zero_extend(struct zero_file *fp, size_t size)
{
	if (size > fp->size) {
		size += ZERO_FRGSIZ;
		fp->data = realloc(fp->data, size);
		fp->size = size;
	}
}

ssize_t zero_pread(struct zero_file *fp, void *buf, size_t len, off_t off)
{
	zero_extend(fp, off + len);
	memcpy(buf, fp->data + off, len);
	return len;
}

ssize_t zero_pwrite(struct zero_file *fp, const void *buf, size_t len, off_t off)
{
	zero_extend(fp, off + len);
	memcpy(fp->data + off, buf, len);
	return len;
}

off_t zero_lseek(struct zero_file *fp, off_t now_off, off_t off, int whence)
{
	switch (whence) {
	case 0: break;
	case 1: off += now_off; break;
	case 2: off = fp->size + off; break;
	default: return -ENOTSUP;
	};
	CLMIN(off, 0);
	//CLMAX(off, fp->size);
	return off;
}

struct zero_file *zero_new_open(unsigned int flags)
{
	/*struct zero_file *fp = malloc(sizeof(struct zero_file));
	fp->id = conn;
	BUG_ON(NULL != idmap_add(&ftab, fp));*/
	struct zero_file *fp = zalloc(sizeof(struct zero_file));
	//static char data[] = "Hello, World!\n";
	fp->data = zalloc(ZERO_FRGSIZ);
	fp->size = ZERO_FRGSIZ;
	return fp;
}

void zero_close(struct zero_file *fp)
{
	free(fp->data);free(fp);//fput(fp)
}

void zero_mpt_close(struct mpage_table *mpt)
{
	//fput(mpt->fp);
	free(mpt);
}

int zero_mpt_msync(struct mpage_table *mpt, size_t off, size_t size)
{
	void *page = mpt_lookup_mpage(mpt, off / PageSize);
	BUG_ON(page == NULL);
	zero_pwrite(mpt->fp, page + off % PageSize, size, mpt->base + off);
	return 0;
}

int zero_mpt_fault(struct mpage_table *mpt, size_t off, int errcd, void **ppage)
{
	void *page = amalloc(PageSize, PageSize);
	BUG_ON(mpt_lookup_mpage(mpt, off / PageSize) != NULL);
	mpt_new_mpage(mpt, off / PageSize, page);
	BUG_ON((uintptr_t)page & PageLomask);
	zero_pread(mpt->fp, page, PageSize, mpt->base + off);
	*ppage = page;
	return 0;
}

struct mpage_table *zero_mpt_open(struct zero_file *fp, off_t base, size_t size, unsigned int flags)
{
	struct mpage_table *mpt = zalloc(sizeof(struct mpage_table));

	mpt->flags = flags;
	mpt->size = size;
	mpt->base = base;
	mpt->fp = fp;
	return mpt;
}

void zero_mpt_serve_ipc(struct mpage_table *mpt)
{
	int nr = ipc_getw();

	switch (nr) {

	case _FILE_msync:
	{
		size_t inoff = ipc_getw();
		size_t size = ipc_getw();
		//printk("zero_msync(%d, %d)", inoff, size);
		int succ = zero_mpt_msync(mpt, inoff, size);
		ipc_rewindw(succ);
	} break;

	case _FILE_fault:
	{
		size_t inoff = ipc_getw();
		int errcd = ipc_getw();
		//printk("zero_fault(%d, %d)", inoff, errcd);
		void *page = NULL;
		int succ = zero_mpt_fault(mpt, inoff, errcd, &page);
		ipc_rewindw(succ);
		ipc_putw((uintptr_t)page);
	} break;

	default:
		ipc_rewindw(-ENOTSUP);
	}
	ipc_reply();
}

void zero_serve_ipc(struct zero_file *fp)
{
	switch (ipc_gettype()) {
	case 2: return zero_mpt_serve_ipc((void*)fp);
	}

	int nr = ipc_getw();

	switch (nr) {

	case _FILE_mmap:
	{
		off_t base = ipc_getoffset();
		size_t size = ipc_getw();
		unsigned int flags = ipc_getw();
		//printk("zero_mmap(%d)", base);
		struct mpage_table *mpt = zero_mpt_open(fp, base, size, flags);
		if (mpt != NULL) {
			ipc_setbadge((uintptr_t)mpt);
			ipc_settype(2);
			ipc_rewindw(0);
		} else {
			ipc_rewindw(errno);
		}
	} break;

	case _FILE_pread:
	{
		size_t len = ipc_getw();
		off_t off = ipc_getw();
		//printk("zero_pread(%d, %d)", len, off);
		ipc_seek_setw(1);
		void *buf = ipc_getbuf(&len);
		ssize_t ret = zero_pread(fp, buf, len, off);
		ipc_rewindw(ret);
	} break;

	case _FILE_pwrite:
	{
		size_t len = ipc_getw();
		off_t off = ipc_getw();
		//printk("zero_pwrite(%d, %d)", len, off);
		const void *buf = ipc_getbuf(&len);
		ssize_t ret = zero_pwrite(fp, buf, len, off);
		ipc_rewindw(ret);
	} break;

	case _FILE_read:
	{
		size_t len = ipc_getw();
		//printk("zero_read(%d)", len);
		ipc_seek_setw(1);
		void *buf = ipc_getbuf(&len);
		off_t off = ipc_getoffset();
		ssize_t ret = zero_pread(fp, buf, len, off);
		if (ret > 0)
			off += ret;
		ipc_setoffset(off);
		ipc_rewindw(ret);
	} break;

	case _FILE_write:
	{
		size_t len = ipc_getw();
		//printk("zero_write(%d)", len);
		const void *buf = ipc_getbuf(&len);
		off_t off = ipc_getoffset();
		ssize_t ret = zero_pwrite(fp, buf, len, off);
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
		off_t ret = zero_lseek(fp, now_off, off, whence);
		if (ret >= 0)
			ipc_setoffset(ret);
		ipc_rewindw(ret);
	} break;

	default:
		ipc_rewindw(-ENOTSUP);
	}
	ipc_reply();
}

const int libh4_serve_id = SVID_ZERO;

int main(void)
{
	static char buffer[4096*64];
	liballoc_set_memory(buffer, sizeof(buffer));

	while (1) {
		ipc_recv();
		void *p = (void*)ipc_getbadge();
		if (ipc_isclose()) {
			BUG();
			/*switch (ipc_gettype())｛
			case 2: zero_mpt_close(p);
			default: zero_close(p);
			}*/
		} else {
			if (p == NULL) {
				unsigned int flags = 0;//TOD
				p = zero_new_open(flags);
				ipc_setbadge((uintptr_t)p);
			}
			zero_serve_ipc(p);
		}
	}
}
