#include "dir.h"
#include <string.h>
#include <errno.h>
#include <bug.h>
#include "error.h"

static bool dir_lookup(vn_t *dir, const char *name, de_t *e)
{
	int n;
	for (n = 0; n < dir->size / DESIZE; n++) {
		if (DESIZE != vread(dir, e, DESIZE, n * DESIZE))
			return -EIO;
		if (!ecmpname(e, name))
			return 0;
	}
	return -ENOENT;
}

static char *cutpath(const char *path, char *name)
{
	char *end = strchrl(path, '/');
	if (end - path > NAME_MAX)
		return NULL;
	strncpy(name, path, end - path);
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
		path = cutpath(path, name);
		if (*path == 0) {
			if (parent) {
				*ppv = dir;
				*endp = (char*)path;
				return 0;
			}
			BUG_ON(!runned);
			goto out;
		}

		ret = dir_lookup(dir, name, e);
		if (ret < 0)
			return ret;

		if (runned)
			free(dir);
		dir = vopendir(dir->sb, e);
		if (dir == NULL)
			return -errno;

		runned = true;
	}

out:
	if (runned)
		free(dir);
	return 0;
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
