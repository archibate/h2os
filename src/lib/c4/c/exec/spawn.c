#include <spawn.h>
#include <stdlib.h>
#include <fcntl.h>
#include <string.h>
#include <errno.h>
#include <stddef.h>

int spawn(const char *name, char *const *argv, char *const *envp, const struct spawnattr *sat)
{
	char path[MAX_PATH], *ps;
	int ret, le;
	char *default_argv[] = {(char*)name, NULL};
	if (argv == NULL)
		argv = default_argv;
	if (strchr(name, '/'))
		return execvp(name, argv);
	if (!(ps = getenv("PATH")) || !*ps)
		return -ENOENT;
	while (1) {
		le = strchrl(ps, ':') - ps;
		memcpy(path, ps, le);
		path[le++] = '/';
		strcpy(path + le, name);
		ret = spawnp(path, argv, envp, sat);
		if (ret >= 0 || ret != -ENOENT)
			break;
	} while ((ps += le + 1)[-1]);
	return ret;
}
