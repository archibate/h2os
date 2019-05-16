#include "vn.h"
#include "sb.h"
#include "vn.h"
#include "de.h"
#include <errno.h>
#include <malloc.h>
#include <numtools.h>
#include <h4/file/api.h>
#include "error.h"
#include <bug.h>
//#include <printk.h>//
// TODO: check http://blog.chinaunix.net/uid-23141914-id-211775.html for more
// details about FAT cluster cache (LRU)!!!!!!!!!

#if 0
static void *hook_malloc(size_t size)
{
	printk("fs: vn.c: hook_malloc(%d)", size);
	return malloc(size);
}
#define malloc hook_malloc
#endif

vn_t *vdup(vn_t *v)
{
	v->refcount++;
	return v;
}

vn_t *__vopen(sb_t *sb, de_t *e)
{
#if 0//{{{
	if (flags & O_WRONLY) {
		if (e->attr & T_RO)
			return error(EPERM);
		return error(EROFS);
	}
#endif//}}}
	vn_t *v = malloc(sizeof(vn_t));
	v->attr = e->attr;
	v->size = e->size;
	v->sb = sb;
	v->type = VN_REGFAT;

	v->clus_start = egetclus(e);

	return v;
}

vn_t *sb_openroot(sb_t *sb)
{
	vn_t *v = malloc(sizeof(vn_t));
	v->sb = sb;
	v->size = sb->root_ents * DESIZE;
	v->type = VN_ROOTDIR;
	v->attr = T_DIR;
	return v;
}

int vclose(vn_t *v)
{
	if (!--v->refcount)
		free(v);
	return 0;
}

vn_t *vopenfile(sb_t *sb, de_t *e)
{
	if (e->attr & T_DIR)
		return error(EISDIR);

	return __vopen(sb, e);
}

vn_t *vopendir(sb_t *sb, de_t *e)
{
	if (!(e->attr & T_DIR))
		return error(ENOTDIR);

	return __vopen(sb, e);
}

extern ssize_t __vrw_regfat(vn_t *v, void *buf, size_t len, off_t off, bool wr);
#if 0 // {{{
static ssize_t __vrw_regfat(vn_t *v, void *buf, size_t len, off_t off, bool wr)
{
	//printk("!!vvvvv=%p", v);//

	int i, mi = -1;
	uint32_t min = v->size;
	size_t bsize = v->sb->bsize;
	uint32_t blkn = off / bsize;
	uint32_t ioff = off % bsize;
	for (i = 0; i < CBUFMAX; i++) {
		int32_t m = v->blkn[i] - blkn;
		if (m == 0) {
			mi = i;
			break;

		} else if (m > 0 && m < min) {
			mi = i;
			min = m;
		}
	}
	BUG_ON(mi == -1);
	uint32_t clus = v->clus[mi];
	//printk("clus=%d", clus);//
	for (; v->blkn[mi] < blkn; v->blkn[mi]++) {
		clus = v->sb->fat[clus];
		if (clus >= 0xfffffff0)
			return -EIO;
	}

	CLMAX(len, v->size);
	size_t n = len;
	while (n > 0) {
		size_t m = n;
		CLMAX(m, bsize - ioff);
		off_t cbase = v->sb->begin + bsize * clus;

		if (wr)
			pwrite(v->sb->hd, buf, m, cbase + ioff);
		else
			pread(v->sb->hd, buf, m, cbase + ioff);

		//printk("hdrw(%#x, %#x)", m, cbase + ioff);//
		//printk("buf[0] = %c", ((char*)buf)[0]);//

		n -= m;
		if (m + ioff == bsize) {
			clus = v->sb->fat[clus];
			//printk("clus new=%d", clus);//
			blkn++;
		}
		ioff = 0;
	}

	v->blkn[mi] = blkn;
	v->clus[mi] = clus;

	return len - n;
}
#endif // }}}

//extern const char *g_path;//
static ssize_t __vrw_rootdir(vn_t *v, void *buf, size_t len, off_t off, bool wr)
{
		//printk("1[%p:%s][%p:%s]", g_path, g_path, buf, buf);//
		//unsigned long sp; asm volatile ("mov %%esp, %0" : "=r" (sp));//
		//printk("%p %p %p %p", g_path, buf, buf + len, sp);//
		//int ret;
	if (wr)
		return pwrite(v->sb->hd, buf, len, v->sb->root_beg + off);
	else
		return pread(v->sb->hd, buf, len, v->sb->root_beg + off);
		//printk("2[%p:%s][%p:%s]", g_path, g_path, buf, buf);//
		//return ret;
}

static ssize_t __vrw(vn_t *v, void *buf, size_t len, off_t off, bool wr)
{
	if (off < 0)
		return -EINVAL;
	if (!wr) {
		if (off > v->size)
			return -EINVAL;
		CLMAX(len, v->size - off);
	}

	switch (v->type) {
	case VN_REGFAT:
		return __vrw_regfat(v, buf, len, off, wr);
	case VN_ROOTDIR:
		return __vrw_rootdir(v, buf, len, off, wr);
	default:
		BUG();
	};
}

ssize_t vread(vn_t *v, void *buf, size_t len, off_t off)
{
	//printk("vread(%p, %p, %d, %d)", v, buf, len, off);//
	return __vrw(v, buf, len, off, false);
}

ssize_t vwrite(vn_t *v, const void *buf, size_t len, off_t off)
{
	if (v->attr & T_RO)
		return -EPERM;
	if (v->sb->rofs)
		return -EROFS;

	return __vrw(v, (void*)buf, len, off, true);
}
