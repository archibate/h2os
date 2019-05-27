#include <spawn.h>

extern char *const *__crt_envp;

static struct spawnattr defl_sat =
{
	.stdio = {0,1,2},
};

int spawnp(const char *path, char *const *argv, char *const *envp, const struct spawnattr *sat)
{
	return _spawn(path, argv, envp ? envp : __crt_envp, sat ? sat : &defl_sat);
}
