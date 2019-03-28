#include "vn.h"
#include <errno.h>
#include <malloc.h>
#include <numtools.h>
#include <h4/file/api.h>
#include "error.h"
#include <bug.h>

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

	uint32_t clus = egetclus(e);
	int i;
	for (i = 0; i < CBUFMAX; i++) {
		v->clus[i] = clus;
		v->blkn[i] = 0;
	}

	return v;
}

int vclose(vn_t *v)
{
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

static ssize_t __vrw(vn_t *v, void *buf, size_t len, off_t off, bool wr)
{
	int i;
	size_t bsize = v->sb->bsize;
	uint32_t blkn = off / bsize;
	uint32_t ioff = off % bsize;
	for (i = 0; i < CBUFMAX; i++) {
		if (v->blkn[i] <= blkn)
			break;
	}
	BUG_ON(i == CBUFMAX);
	uint32_t clus = v->clus[i];
	v->blkn[0] = v->blkn[i];
	for (; v->blkn[0] < blkn; v->blkn[0]++) {
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
			pwrite(v->sb->hd, buf, len, cbase + m);
		else
			pread(v->sb->hd, buf, len, cbase + m);
		n -= m;
		if (m + ioff == bsize) {
			clus = v->sb->fat[clus];
			v->blkn[0]++;
		}
	}

	v->clus[0] = clus;

	return 0;
}

ssize_t vread(vn_t *v, void *buf, size_t len, off_t off)
{
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
