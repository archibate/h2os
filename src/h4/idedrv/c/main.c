// https://github.com/archibate/OS67/blob/master/drv/kb.c
#include <h4/sys/types.h>
#include <h4/sys/ipc.h>
#include <h4/servers.h>
#include <h4/file/sysnr.h>
#include <memory.h>
#include <malloc.h>
#include <stddef.h>
#include <errno.h>
#include "ide.h"
#include <bug.h>
#include <panic.h>
#include <printk.h>
#include <numtools.h>
#include <c4/liballoc.h>
#include <l4/machine/mmu/page.h>
#include <list.h>

static const dev_t ide_dev = 0;
char ide_buf[BSIZE];
static blkno_t ide_blkno = -1;

ssize_t ide_pread(void *buf, size_t len, off_t off)
{
	blkno_t blkno = off / BSIZE;
	size_t skip = off % BSIZE;
	size_t rlen = len;
	CLMAX(rlen, BSIZE - skip);

	//printk("!!!!buf=%p", buf);//
	if (ide_blkno != blkno) {
		ide_rdblk(ide_dev, blkno, &ide_buf);
	}
	ide_blkno = blkno++;
	//printk("!!!buf=%p", buf);//
	memcpy(buf, ide_buf + skip, rlen);
	buf += rlen;

	//printk("!!buf=%p", buf);//
	while (rlen + BSIZE < len) {
		ide_rdblk(ide_dev, blkno++, buf);
		buf += BSIZE;
		rlen += BSIZE;
	}
	//printk("!buf=%p", buf);//

	size_t rest = len - rlen;
	if (rest != 0) {
		ide_rdblk(ide_dev, blkno, &ide_buf);
	//printk("buf=%p", buf);//
		memcpy(buf, ide_buf, rest);
		ide_blkno = blkno++;
	}
	//printk("buf copyin done");//
	return len;
}

ssize_t ide_pwrite(const void *buf, size_t len, off_t off)
{
	blkno_t blkno = off / BSIZE;
	size_t skip = off % BSIZE;
	size_t rlen = len;
	CLMAX(rlen, BSIZE - skip);

	if (ide_blkno != blkno && !(skip == 0 || rlen == BSIZE)) {
		ide_rdblk(ide_dev, blkno, &ide_buf);
	}
	ide_blkno = blkno;
	memcpy(ide_buf + skip, buf, rlen);
	ide_wrblk(ide_dev, blkno++, &ide_buf);
	buf += rlen;

	while (rlen + BSIZE < len) {
		ide_wrblk(ide_dev, blkno++, buf);
		buf += BSIZE;
		rlen += BSIZE;
	}

	size_t rest = len - rlen;
	if (rest != 0) {
		if (rlen != BSIZE) {
			ide_rdblk(ide_dev, blkno, &ide_buf);
		}
		memcpy(ide_buf, buf, rest);
		ide_wrblk(ide_dev, blkno, &ide_buf);
		ide_blkno = blkno++;
	}
	return len;
}

off_t ide_lseek(off_t now_off, off_t off, int whence)
{
	switch (whence) {
	case 0: break;
	case 1: off += now_off; break;
	case 2: return -ENOTSUP;
	default: return -ENOTSUP;
	};
	CLMIN(off, 0);
	return off;
}

/////////////////////////////////

struct mpage_table
{
	struct hlist_head head;
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


void ide_mpt_close(struct mpage_table *mpt)
{
	//fput(mpt->fp);
	free(mpt);
}

int ide_mpt_msync(struct mpage_table *mpt, size_t off, size_t size)
{
	void *page = mpt_lookup_mpage(mpt, off / PageSize);
	BUG_ON(page == NULL);
	ide_pwrite(page + off % PageSize, size, mpt->base + off);
	return 0;
}

int ide_mpt_fault(struct mpage_table *mpt, size_t off, int errcd, void **ppage)
{
	void *page = amalloc(PageSize, PageSize);
	BUG_ON(mpt_lookup_mpage(mpt, off / PageSize) != NULL);
	mpt_new_mpage(mpt, off / PageSize, page);
	BUG_ON((uintptr_t)page & PageLomask);
	ide_pread(page, PageSize, mpt->base + off);
	*ppage = page;
	return 0;
}

struct mpage_table *ide_mpt_open(off_t base, size_t size, unsigned int flags)
{
	struct mpage_table *mpt = zalloc(sizeof(struct mpage_table));

	mpt->flags = flags;
	mpt->size = size;
	mpt->base = base;
	return mpt;
}

void ide_mpt_serve_ipc(void)
{
	struct mpage_table *mpt = (void*)ipc_getbadge();

	int nr = ipc_getw();

	switch (nr) {

	case _FILE_msync:
	{
		size_t inoff = ipc_getw();
		size_t size = ipc_getw();
		//printk("ide_msync(%d, %d)", inoff, size);
		int succ = ide_mpt_msync(mpt, inoff, size);
		ipc_rewindw(succ);
	} break;

	case _FILE_fault:
	{
		size_t inoff = ipc_getw();
		int errcd = ipc_getw();
		//printk("ide_fault(%d, %d)", inoff, errcd);
		void *page = NULL;
		int succ = ide_mpt_fault(mpt, inoff, errcd, &page);
		ipc_rewindw(succ);
		ipc_putw((uintptr_t)page);
	} break;

	default:
		ipc_rewindw(-ENOTSUP);
	}
	ipc_reply();
}

void ide_serve_ipc(void)
{
	switch (ipc_gettype()) {
	case 2: return ide_mpt_serve_ipc();
	}

	int nr = ipc_getw();

	switch (nr) {

	case _FILE_mmap:
	{
		off_t base = ipc_getoffset();
		size_t size = ipc_getw();
		unsigned int flags = ipc_getw();
		//printk("ide_mmap(%d)", base);
		struct mpage_table *mpt = ide_mpt_open(base, size, flags);
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
		//printk("ide_pread(%d, %d)", len, off);
		ipc_seek_setw(1);
		void *buf = ipc_getbuf(&len);
		ssize_t ret = ide_pread(buf, len, off);
		ipc_rewindw(ret);
	} break;

	case _FILE_pwrite:
	{
		size_t len = ipc_getw();
		off_t off = ipc_getw();
		//printk("ide_pwrite(%d, %d)", len, off);
		const void *buf = ipc_getbuf(&len);
		//printk("ide_pwrite: buf=[%s], len=%d, off=%d", buf, len, off);
		ssize_t ret = ide_pwrite(buf, len, off);
		ipc_rewindw(ret);
	} break;

	case _FILE_read:
	{
		size_t len = ipc_getw();
		//printk("ide_read(%d)", len);
		ipc_seek_setw(1);
		void *buf = ipc_getbuf(&len);
		off_t off = ipc_getoffset();
		ssize_t ret = ide_pread(buf, len, off);
		if (ret > 0)
			off += ret;
		ipc_setoffset(off);
		ipc_rewindw(ret);
	} break;

	case _FILE_write:
	{
		size_t len = ipc_getw();
		//printk("ide_write(%d)", len);
		const void *buf = ipc_getbuf(&len);
		off_t off = ipc_getoffset();
		ssize_t ret = ide_pwrite(buf, len, off);
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
		off_t ret = ide_lseek(now_off, off, whence);
		if (ret >= 0)
			ipc_setoffset(ret);
		ipc_rewindw(ret);
	} break;

	default:
		ipc_rewindw(-ENOTSUP);
	}
	ipc_reply();
}

const int libh4_serve_id = SVID_IDEDRV;

int main(void)
{
	static char buffer[4096*32];
	liballoc_set_memory(buffer, sizeof(buffer));

	while (1) {
		ipc_recv();
		ide_serve_ipc();
	}
}
