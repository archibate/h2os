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
	vn_t *v = zalloc(sizeof(vn_t));
	v->attr = e->attr;
	v->size = e->size;
	v->sb = sb;
	v->type = VN_REGFAT;
	v->refcount = 1;

	v->clus_start = egetclus(e);

	return v;
}

vn_t *sb_openroot(sb_t *sb)
{
	vn_t *v = zalloc(sizeof(vn_t));
	v->sb = sb;
	v->size = sb->root_ents * DESIZE;
	v->type = VN_ROOTDIR;
	v->attr = T_DIR;
	v->refcount = 1;
	v->exflags = O_RDWR | O_DIR;
	return v;
}

void vupdate(vn_t *v)
{
	if (v->type != VN_REGFAT)
		return;

	de_t e;
	BUG_ON(pread(v->sb->hd, &e, DESIZE, v->dehdoff) != DESIZE);

	esetclus(&e, v->clus_start);
	e.size = v->size;

	BUG_ON(pwrite(v->sb->hd, &e, DESIZE, v->dehdoff) != DESIZE);
}

int vdeunlink(vn_t *v)
{
	if (v->type != VN_REGFAT)
		return -EPERM;

	if (v->refcount > 1)
		return -EBUSY;

	if (v->sb->rofs)
		return -EROFS;

	de_t e;
	memset(&e, 0, sizeof(e));
	BUG_ON(pwrite(v->sb->hd, &e, DESIZE, v->dehdoff) != DESIZE);

	return 0;
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
