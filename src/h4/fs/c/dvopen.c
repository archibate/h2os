#include "dir.h"
#include "de.h"
#include <string.h>
#include <errno.h>
#include <bug.h>
#include "error.h"
#include <printk.h>//

static int make_dirent(de_t *e, const char *name, unsigned int flags)
{
	memset(e, 0, sizeof(e));
	e->clus_hi = 0xffff;
	e->clus_lo = 0xfffa;
	e->attr = flags & 0xff;
	return esetname(e, name);
}

vn_t *dir_vopen(vn_t *dir, const char *path, unsigned int flags)
{
	de_t e;
	int ret;

	if (!*strskipin(path, "/"))
		return vdup(dir);

#if 0 // {{{
	if (!(flags & O_CREAT)) {
		ret = dir_gete(dir, path, &e);
		if (ret < 0)
			return error(ret);
	} else {
		BUG();// for now
	}
#else // }}}
	char name[NAME_MAX+1];
	vn_t *p = dir_getpv(dir, path, name);
	if (!p)
		return NULL;

	ret = dir_lookup(p, name, &e);
	if ((flags & O_CREAT) && ret == -ENOENT) {
		if (dir->sb->rofs)
			return error(EROFS);

		ret = make_dirent(&e, name, flags);
		//printk("e->name=[%s]", e.name);
		if (ret >= 0)
			ret = dir_addent(p, &e);
		BUG_ON(dir_lookup(p, name, &e) < 0);
		if (ret < 0)
			return error(ret);

	} else if (ret < 0)
		return error(ret);

	if ((e.attr & T_RO) && (flags & O_WRONLY))
		return error(EPERM);
#endif

	vn_t *v;
	if (flags & O_DIR)
		v = vopendir(dir->sb, &e);
	else
		v = vopenfile(dir->sb, &e);

	if (v != NULL)
		v->exflags = flags;

	return v;
}
