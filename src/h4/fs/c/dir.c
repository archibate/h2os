#include "dir.h"
#include "de.h"
#include <string.h>
#include <errno.h>
#include <bug.h>
#include "error.h"
#include <printk.h>//

//extern const char *g_path;//

int dir_addent(vn_t *dir, de_t *e)
{
	de_t _e;
	int n;
	for (n = 0; n < dir->size / DESIZE; n++) {
		if (DESIZE != vread(dir, &_e, DESIZE, n * DESIZE))
			return -EIO;
		if (_e.name[0] == 0)
			break;
	}
	if (DESIZE != vwrite(dir, e, DESIZE, n * DESIZE))
		return -EIO;
		//printk("!!!made_dirent%d!!!", ret);
	return 0;
}

int dir_lookup(vn_t *dir, const char *name, de_t *e)
{
	//printk("dir_lookup(%s)", name);//
	int n;
	//printk("!0[%s]", g_path);
	for (n = 0; n < dir->size / DESIZE; n++) {
		//printk("!!");//
		//printk("%p", g_path);//
		//printk("!A[%s]", g_path);//
		if (DESIZE != vread(dir, e, DESIZE, n * DESIZE))
			return -EIO;
		//printk("!!!!dir_lookup:%d", dir->lastpos);
		//printk("!B[%s]", g_path);//
		//if (e->name[0]) printk("dir_lookup: %.11s", e->name);//
		//printk("!!!%d", e->attr);//
		if (!ecmpname(e, name))
			return 0;
	}
	//printk("!!!!");//
	return -ENOENT;
}

#if 0 // {{{
static int __dir_getve(vn_t *dir, const char *path, de_t *e,
		vn_t **ppv, char **endp, bool parent)
{
	char name[NAME_MAX+1];
	const char *end;
	int ret;
	bool runned = false;
	//printk("!dg[%p:%s]", path, path);
	
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

		if (parent && *path == 0) {
			*ppv = dir;
			*endp = (char*)path;
			return 0;
		}

		//g_path = path;
		ret = dir_lookup(dir, name, e);
		//g_path = NULL;
		if (ret < 0)
			return ret;

		sb_t *sb = dir->sb;
		if (runned)
			free(dir);

		if (*path == 0 && !parent) {
			//printk("!!!!!!!!!pe");
			return 0;
		}

		dir = vopendir(sb, e);
		//printk("e->name=[%s], dir=%p, errno=%d", e->name, dir, errno);
		if (dir == NULL) {
			BUG_ON(errno <= 0);
			return -errno;
		}

		runned = true;
	}
}

int dir_gete(vn_t *dir, const char *path, de_t *e)
{
	return __dir_getve(dir, path, e, NULL, NULL, false);
}
#endif // }}}
