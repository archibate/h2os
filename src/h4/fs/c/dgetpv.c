#include "dir.h"
#include "de.h"
#include <string.h>
#include <errno.h>
#include <bug.h>
#include "error.h"
#include <printk.h>//

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

vn_t *dir_getpv(vn_t *dir, const char *path, char *name)
{
	sb_t *sb = dir->sb;
	de_t e;
	int ret;
	dir = vdup(dir);

	while (1) {
		path = cutpath(path, name);
		if (path == NULL)
			return error(ENAMETOOLONG);
		if (!*strskipin(path, "/"))
			break;

		ret = dir_lookup(dir, name, &e);
		if (ret < 0)
			return error(ret);

		vclose(dir);
		dir = vopendir(sb, &e);
	}
	return dir;
}
