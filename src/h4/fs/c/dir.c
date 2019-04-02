#include "dir.h"
#include <string.h>
#include <errno.h>
#include <bug.h>
#include "error.h"
#include <printk.h>//

static int dir_lookup(vn_t *dir, const char *name, de_t *e)
{
	//printk("dir_lookup(%s)", name);//
	int n;
	for (n = 0; n < dir->size / DESIZE; n++) {
		//printk("!!");//
		if (DESIZE != vread(dir, e, DESIZE, n * DESIZE))
			return -EIO;
		//if (e->name[0]) printk("dir_lookup: %.11s", e->name);//
		//printk("!!!");//
		if (!ecmpname(e, name))
			return 0;
	}
	//printk("!!!!");//
	return -ENOENT;
}

static char *cutpath(const char *path, char *name)
{
	path = strskipin(path, "/");
	char *end = strchrl(path, '/');
	//printk("cputpath(%s, %s)", path, end);//

	if (end - path > NAME_MAX)
		return NULL;

	strncpy(name, path, end - path);
	//printk("cputpath: name=%s", name);//

	return end;
}

static int __dir_getve(vn_t *dir, const char *path, de_t *e,
		vn_t **ppv, char **endp, bool parent)
{
	char name[NAME_MAX+1];
	const char *end;
	int ret;
	bool runned = false;
	
	while (1) {
		if (*path == 0) {
			if (parent) {
				*ppv = vdup(dir);
				*endp = (char*)path;
				if (runned)
					free(dir);
				return 0;
			}
			if (!runned)
				return -EINVAL;
		}
		path = cutpath(path, name);
		if (path == NULL)
			return -ENAMETOOLONG;

		ret = dir_lookup(dir, name, e);
		if (ret < 0)
			return ret;

		sb_t *sb = dir->sb;
		if (runned)
			free(dir);

		if (*path == 0 && !parent)
			return 0;

		dir = vopendir(sb, e);
		if (dir == NULL)
			return -errno;

		runned = true;
	}
}

int dir_gete(vn_t *dir, const char *path, de_t *e)
{
	return __dir_getve(dir, path, e, NULL, NULL, false);
}

vn_t *dir_getpv(vn_t *dir, const char *path, char **endp)
{
	de_t e;
	vn_t *v;
	errno = -__dir_getve(dir, path, &e, &v, endp, true);
	return errno ? NULL : v;
}

vn_t *dir_vopen(vn_t *dir, const char *path, unsigned int flags)
{
	if (!(flags & O_CREAT)) {
		de_t e;
		errno = dir_gete(dir, path, &e);
		if (errno)
			return NULL;

		if ((e.attr & T_RO) && (flags & O_WRONLY))
			return error(EPERM);

		vn_t *v;
		if (flags & O_DIR)
			v = vopendir(dir->sb, &e);
		else
			v = vopenfile(dir->sb, &e);

		if (v != NULL)
			v->exflags = flags;

		return v;

	} else {
		if (dir->sb->rofs)
			return error(EROFS);

		BUG(); // file creation not impelemented
	}
}