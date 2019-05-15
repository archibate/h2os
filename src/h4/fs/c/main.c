#define USEFAT 1
#define USEMPT 1
#include <h4/sys/types.h>
#include <h4/sys/ipc.h>
#include <h4/fs/sysnr.h>
#include <h4/fs/defines.h>
#include <h4/file/sysnr.h>
#include <h4/servers.h>
#include <malloc.h>
#include <errno.h>
#include <string.h>
#include <case.h>
#include <printk.h>
#include <bug.h>
#include <panic.h>
#include <h4/fs/oflags.h>
#include <l4/machine/mmu/page.h>
#include <numtools.h>
#ifdef USEFAT
#include "vn.h"
#include "dir.h"
#define IPCT_FILE  1
#endif
#ifdef USEMPT
#include <list.h>
#define IPCT_MPTAB 2
#endif

#if 0
static void *hook_malloc(size_t size)
{
	printk("fs: main.c: hook_malloc(%d)", size);
	return malloc(size);
}
#define malloc hook_malloc
#endif

int dev_resolve(const char *name)
{
	if (!strcmp(name, "zero")) 
		return SVID_ZERO;
	else if (!strcmp(name, "cons")) 
		return SVID_CONS;
	else if (!strcmp(name, "keybd")) 
		return SVID_KEYBD;
	else if (!strcmp(name, "hda")) 
		return SVID_IDEDRV;
	else if (!strcmp(name, "hello")) 
		return SVID_HELLO;
	else 
		return -ENOENT;
}

int do_open(const char *path, unsigned int flags)
{
	//printk("do_open(%s, %d)", path, flags);

	if (!strncmp(path, "/dev/", strlen("/dev/")))
		return dev_resolve(path + strlen("/dev/"));

#ifdef USEFAT
	extern vn_t *fat_root;
	vn_t *v = dir_vopen(fat_root, path, flags);
	if (!v)
		return -errno;

	ipc_setbadge((uintptr_t)v);
	ipc_settype(IPCT_FILE);
	return 0;
#else
	return -ENOENT;
#endif
}

#ifdef USEFAT
ssize_t file_write(vn_t *v, const char *buf, size_t len, off_t off)
{
	if (!(v->exflags & O_WRONLY))
		return -EPERM;

	return vwrite(v, buf, len, off);
}

ssize_t file_read(vn_t *v, char *buf, size_t len, off_t off)
{
	if (!(v->exflags & O_RDONLY))
		return -EPERM;

	return vread(v, buf, len, off);
}

off_t file_lseek(vn_t *v, off_t now_off, off_t off, int whence)
{
	switch (whence) {
	case 0: break;
	case 1: off += now_off; break;
	case 2: off = v->size + off; break;
	default: return -ENOTSUP;
	};
	CLMIN(off, 0);
	CLMAX(off, v->size);
	return off;
}

void file_close(vn_t *v)
{
	free(v);
}

#ifdef USEMPT
struct mpage_table
{
	struct hlist_head head;
	vn_t *fp;
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

void file_mpt_close(struct mpage_table *mpt)
{
	//fput(mpt->fp);
	free(mpt);
}

int file_mpt_msync(struct mpage_table *mpt, size_t off, size_t size)
{
	void *page = mpt_lookup_mpage(mpt, off / PageSize);
	BUG_ON(page == NULL);
	file_write(mpt->fp, page + off % PageSize, size, mpt->base + off);
	return 0;
}

int file_mpt_fault(struct mpage_table *mpt, size_t off, int errcd, void **ppage)
{
	void *page = amalloc(PageSize, PageSize);
	//printk("file_mpt_fault: off=%d, page=%p", off, page);
	BUG_ON(mpt_lookup_mpage(mpt, off / PageSize) != NULL);
	mpt_new_mpage(mpt, off / PageSize, page);
	BUG_ON((uintptr_t)page & PageLomask);
	//printk("file_mpt_fault: file_read(%d)", mpt->base + off);
	file_read(mpt->fp, page, PageSize, mpt->base + off);
	//panic("file: page[0] = %d", ((char*)page)[0]);
	*ppage = page;
	return 0;
}

struct mpage_table *file_mpt_open(vn_t *fp, off_t base, size_t size, unsigned int flags)
{
	struct mpage_table *mpt = zalloc(sizeof(struct mpage_table));

	mpt->flags = flags;
	mpt->size = size;
	mpt->base = base;
	mpt->fp = fp;
	return mpt;
}

void file_mpt_serve_ipc(struct mpage_table *mpt)
{
	int nr = ipc_getw();

	switch (nr) {

	case _FILE_msync:
	{
		size_t inoff = ipc_getw();
		size_t size = ipc_getw();
		//printk("file_msync(%d, %d)", inoff, size);
		int succ = file_mpt_msync(mpt, inoff, size);
		ipc_rewindw(succ);
	} break;

	case _FILE_fault:
	{
		size_t inoff = ipc_getw();
		int errcd = ipc_getw();
		//printk("file_fault(%d, %d)", inoff, errcd);
		void *page = NULL;
		int succ = file_mpt_fault(mpt, inoff, errcd, &page);
		ipc_rewindw(succ);
		ipc_putw((uintptr_t)page);
	} break;

	default:
		ipc_rewindw(-ENOTSUP);
	}
	ipc_reply();
}
#endif

void file_serve_ipc(vn_t *v)
{
	unsigned int nr = ipc_getw();
	//printk("!!!nr=%d", nr);
	switch (nr) {

#ifdef USEMPT
	case _FILE_mmap:
	{
		off_t base = ipc_getoffset();
		size_t size = ipc_getw();
		unsigned int flags = ipc_getw();
		//printk("file_mmap(%d)", base);
		struct mpage_table *mpt = file_mpt_open(v, base, size, flags);
		if (mpt != NULL) {
			ipc_setbadge((uintptr_t)mpt);
			ipc_settype(2);
			ipc_rewindw(0);
		} else {
			ipc_rewindw(errno);
		}
	} break;
#endif

	case _FILE_pread:
	{
		size_t len = ipc_getw();
		off_t off = ipc_getw();
		//printk("file_pread(%d, %d)", len, off);
		ipc_seek_setw(1);
		void *buf = ipc_getbuf(&len);
		ssize_t ret = file_read(v, buf, len, off);
		ipc_rewindw(ret);
	} break;

	case _FILE_pwrite:
	{
		size_t len = ipc_getw();
		off_t off = ipc_getw();
		//printk("file_pwrite(%d, %d)", len, off);
		const void *buf = ipc_getbuf(&len);
		ssize_t ret = file_write(v, buf, len, off);
		ipc_rewindw(ret);
	} break;

	case _FILE_read:
	{
		size_t len = ipc_getw();
		//printk("file_read(%d)", len);
		ipc_seek_setw(1);
		void *buf = ipc_getbuf(&len);
		off_t off = ipc_getoffset();
		ssize_t ret = file_read(v, buf, len, off);
		if (ret > 0)
			off += ret;
		ipc_setoffset(off);
		ipc_rewindw(ret);
	} break;

	case _FILE_write:
	{
		size_t len = ipc_getw();
		//printk("file_write(%d)", len);
		const void *buf = ipc_getbuf(&len);
		off_t off = ipc_getoffset();
		ssize_t ret = file_write(v, buf, len, off);
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
		off_t ret = file_lseek(v, now_off, off, whence);
		if (ret >= 0)
			ipc_setoffset(ret);
		//printk("file_setoffset(%d)", ret);
		ipc_rewindw(ret);
	} break;

	default:
		ipc_rewindw(-ENOTSUP);
	}
	ipc_reply();
}
#endif

const int libh4_serve_id = SVID_FS;

int main(void)
{
	static char buffer[4096*512];
	liballoc_set_memory(buffer, sizeof(buffer));

#ifdef USEFAT
	extern void fat_init(void);
	fat_init();
#endif

	while (1) {
		ipc_recv();

		void *p = (void*)ipc_getbadge();
		switch (ipc_gettype()) {
#ifdef USEFAT
		case IPCT_FILE: file_serve_ipc(p); ipc_reply(); continue;
#endif
#ifdef USEMPT
		case IPCT_MPTAB: file_mpt_serve_ipc(p); ipc_reply(); continue;
#endif
		}

		int nr = ipc_getw();
		switch (nr) {
		CASE(_FS_open) {
			size_t len = MAXPATH;
			unsigned int flags = ipc_getw();
			char *path = ipc_getbuf(&len);
			if (strnlen(path, len) >= MAXPATH) {
				ipc_rewindw(-ENAMETOOLONG);
				BREAK;
			}
			path = strdup(path);
			int ret = do_open(path, flags);
			free(path);
			ipc_rewindw(ret);
		}
		DEFAULT {
			ipc_rewindw(-ENOTSUP);
		}
		}
		ipc_reply();
	}
}
