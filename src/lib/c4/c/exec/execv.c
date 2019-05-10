#include <unistd.h>
#include <stdlib.h>
#include <h4/mm.h>
#include <h4/fs/defines.h>
#include <string.h>
#include <errno.h>

int execv(const char *name, char *const *argv)
{
	char path[MAX_PATH], *ps;
	int ret, le;
	//printk("name [%s]", name);
	if (strchr(name, '/'))
		return execvp(name, argv);
	//printk("using PATH");
	if (!(ps = getenv("PATH")) || !*ps)
		return -ENOENT;
	while (1) {
		le = strchrl(ps, ':') - ps;
		memcpy(path, ps, le);
		path[le++] = '/';
		strcpy(path + le, name);
		ret = execvp(path, argv);
		if (ret != -ENOENT)
			return ret;
	} while ((ps += le + 1)[-1]);
	return ret;
}
