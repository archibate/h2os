#include <spawn.h>

extern char *const *__crt_envp;

int spawnp(const char *path, char *const *argv, char *const *envp, const struct spawnattr *sat)
{
	return _spawn(path, argv, envp ? envp : __crt_envp);
}
