#define USEPIPE
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
#include "dir.h"
#include "vn.h"
#ifdef USEPIPE
#include "pipe.h"
#define IPCT_PIPE  3
#endif
#include <list.h>
#define IPCT_FILE  1
#define IPCT_MPTAB 2

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

int do_link(const char *oldpath, const char *newpath)
{
	printk("do_link: [%s]->[%s]", oldpath, newpath);
	extern vn_t *fat_root;
	vn_t *vsrc = dir_vopen(fat_root, oldpath, 0);
	if (!vsrc)
		return -errno;
	uint32_t src_attr = vsrc->attr & 0xff;
	//printk("do_link: src_attr=%d", src_attr);
	vn_t *vdst = dir_vopen(fat_root, newpath, O_CREAT | O_EXCL | src_attr);
	if (!vdst)
		return -errno;
	int ret = vdelink2(vsrc, vdst);
	//printk("do_link: vdelink: %d->%d", vsrc->dehdoff, vdst->dehdoff);
	vclose(vsrc);
	vclose(vdst);
	return ret;
}

int do_unlink(const char *path)
{
	//return dir_unlink(fat_root, path);
	extern vn_t *fat_root;
	vn_t *v = dir_vopen(fat_root, path, 0);
	if (!v)
		return -errno;
	int ret = vdeunlink(v);
	vclose(v);
	return ret;
}

int do_open(const char *path, unsigned int flags)
{
	//printk("do_open(%s, %d)", path, flags);

	if (!strncmp(path, "/dev/", strlen("/dev/")))
		return dev_resolve(path + strlen("/dev/"));

	extern vn_t *fat_root;
	vn_t *v = dir_vopen(fat_root, path, flags);
	if (!v)
		return -errno;

	ipc_setbadge((uintptr_t)v);
	ipc_settype(IPCT_FILE);
	return 0;
}

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

void file_serve_ipc(vn_t *v)
{
	unsigned int nr = ipc_getw();
	//printk("!!!nr=%d", nr);
	switch (nr) {

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

	case _FILE_pread:
	{
		size_t len = ipc_getw();
		off_t off = ipc_getw();
		//printk("file_pread(%d, %d)", len, off);
		ipc_seek_setw(1);
		void *buf = ipc_getbuf(&len);
		void *p = malloc(len);
		memcpy(p, buf, len);
		ssize_t ret = file_read(v, p, len, off);
		free(p);
		ipc_rewindw(ret);
	} break;

	case _FILE_pwrite:
	{
		size_t len = ipc_getw();
		off_t off = ipc_getw();
		//printk("file_pwrite(%d, %d)", len, off);
		const void *buf = ipc_getbuf(&len);
		void *p = malloc(len);
		memcpy(p, buf, len);
		ssize_t ret = file_write(v, p, len, off);
		free(p);
		ipc_rewindw(ret);
	} break;

	case _FILE_read:
	{
		size_t len = ipc_getw();
		//printk("file_read(%d)", len);
		ipc_seek_setw(1);
		void *buf = ipc_getbuf(&len);
		void *p = malloc(len);
		memcpy(p, buf, len);
		off_t off = ipc_getoffset();
		ssize_t ret = file_read(v, p, len, off);
		free(p);
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
		void *p = malloc(len);
		memcpy(p, buf, len);
		off_t off = ipc_getoffset();
		ssize_t ret = file_write(v, p, len, off);
		free(p);
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

#ifdef USEPIPE
int do_pipctl(struct pipe *pip, int iswr)
{
	int otype = ipc_getoffset();
	//int ret = pipe_control(pip, iswr); ipc_rewindw(ret);
	switch (iswr) {
	case 1:
		if (otype != 0)
			return -ENOTSUP;
		ipc_setoffset(2);
		return 0;
	case 0:
		if (otype != 0)
			return -ENOTSUP;
		ipc_setoffset(1);
		return 0;
	case -1:
		switch (otype) {
		case 0:
			return -ENOTSUP;
		case 2:
			//printk("pipe_writer_close!!");
			pip->writer_closed = true;
			break;
		case 1:
			//printk("pipe_reader_close!!");
			pip->reader_closed = true;
			break;
		default:
			BUG();
		};
		return 0;
	}
	return -ENOTSUP;
}

void pipe_serve_ipc(struct pipe *pip)
{
	unsigned int nr = ipc_getw();
	//printk("!!!nr=%d", nr);
	switch (nr) {

	case _FILE_pwrite:
	case _FILE_pread:
	case _FILE_lseek:
	{
		ipc_rewindw(-ESPIPE);
	} break;

#if 0
	case _FILE_read:
	{
		size_t len = ipc_getw();
		//printk("pipe_read(%d)", len);
		ipc_seek_setw(1);
		void *buf = ipc_getbuf(&len);
		ssize_t ret = pipe_read(pip, buf, len);
		ipc_rewindw(ret);
	} break;

	case _FILE_write:
	{
		size_t len = ipc_getw();
		//printk("pipe_write(%d)", len);
		const void *buf = ipc_getbuf(&len);
		ssize_t ret = pipe_write(pip, buf, len);
		ipc_rewindw(ret);
	} break;
#else

	case _FILE_pipctl:
	{
		int iswr = ipc_getw();
		int ret = do_pipctl(pip, iswr);
		ipc_rewindw(ret);
	} break;

	case _FILE_read:
	{
		if (ipc_getoffset() != 1) {
			ipc_rewindw(-EPERM);
			break;
		}
		size_t len = ipc_getw();
		//printk("pipe_read(%d)", len);
		ipc_seek_setw(1);
		void *buf = ipc_getbuf(&len);
		/*void *p = malloc(len);
		memcpy(p, buf, len);*/
		//if (!pipe_empty(pip)) BUG();
		ssize_t ret = pipe_read(pip, buf, len);
		//printk("%p: pipe_read(%d): %d", pip, len, ret);
		//if (ret > 0) printk("pipe_read[%s]", buf);
		//free(p);
		ipc_rewindw(ret);
	} break;

	case _FILE_write:
	{
		if (ipc_getoffset() != 2) {
			ipc_rewindw(-EPERM);
			break;
		}
		size_t len = ipc_getw();
		//printk("pipe_write(%d)", len);
		const void *buf = ipc_getbuf(&len);
		//printk("pipe_write[%s]", buf);
		/*void *p = malloc(len);
		memcpy(p, buf, len);*/
		ssize_t ret = pipe_write(pip, buf, len);
		//printk("%p: pipe_write(%d): %d", pip, len, ret);
		//BUG();
		//BUG_ON(pipe_empty(pip));
		//free(p);
		ipc_rewindw(ret);
	} break;
#endif

	default:
		ipc_rewindw(-ENOTSUP);
	}
	ipc_reply();
}
#endif

char *ipc_getpath(void)
{
	size_t maxlen = MAXPATH;
	char *path = ipc_getbuf(&maxlen);
	size_t len = strnlen(path, maxlen);
	if (len >= maxlen)
		return NULL;
	//BUG_ON(ipc_tell() != 4);
	//printk("%d", ipc_tell());
	//ipc_seek_set(4 + len + 1);
	//ipc_seek_cur(len + 1);
	return strdup(path);
}

const int libh4_serve_id = SVID_FS;

int main(void)
{
	static char buffer[4096*512];
	liballoc_set_memory(buffer, sizeof(buffer));

	extern void fat_init(void);
	fat_init();

	while (1) {
		ipc_recv();

		void *p = (void*)ipc_getbadge();
		switch (ipc_gettype()) {
		case IPCT_FILE: file_serve_ipc(p); ipc_reply(); continue;
		case IPCT_MPTAB: file_mpt_serve_ipc(p); ipc_reply(); continue;
#ifdef USEPIPE
		case IPCT_PIPE: pipe_serve_ipc(p); ipc_reply(); continue;
#endif
		}

		int nr = ipc_getw();
		switch (nr) {
		CASE(_FS_open) {
			unsigned int flags = ipc_getw();
			char *path = ipc_getpath();
			if (path == NULL) {
				ipc_rewindw(-ENAMETOOLONG);
				BREAK;
			}
			int ret = do_open(path, flags);
			free(path);
			ipc_rewindw(ret);
		}
		CASE(_FS_link) {
	//printk("ipc1[%s]", ipc_getbuf(NULL)+1);
			char *oldpath = ipc_getpath();
			if (oldpath == NULL) {
				ipc_rewindw(-ENAMETOOLONG);
				BREAK;
			}
	//printk("ipc2[%s]", ipc_getbuf(NULL)+1);
			ipc_seek_set(4 + strlen(oldpath) + 1);//T
			char *newpath = ipc_getpath();
			if (newpath == NULL) {
				ipc_rewindw(-ENAMETOOLONG);
				BREAK;
			}
			int ret = do_link(oldpath, newpath);
			free(oldpath);
			free(newpath);
			ipc_rewindw(ret);
		}
		CASE(_FS_unlink) {
			char *path = ipc_getpath();
			if (path == NULL) {
				ipc_rewindw(-ENAMETOOLONG);
				BREAK;
			}
			int ret = do_unlink(path);
			free(path);
			ipc_rewindw(ret);
		}
#ifdef USEPIPE
		CASE(_FS_pipe) {
			struct pipe *pip = new_pipe();
			ipc_setbadge((uintptr_t)pip);
			ipc_settype(IPCT_PIPE);
			ipc_setoffset(0);
			ipc_rewindw(0);
		}
#endif
		DEFAULT {
			ipc_rewindw(-ENOTSUP);
		}
		}
		ipc_reply();
	}
}
