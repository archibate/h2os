#include <h4/mm.h>
#include <h4/sys/types.h>
#include <h4/sys/spawn.h>
#include <h4/sys/ipc.h>
#include <h4/fs/defines.h>
#include <h4/filedes.h>
#include <h4/mm/sysnr.h>
#include <h4/servers.h>//

static void ipc_putstr(const char *s, size_t maxlen)
{
	size_t len = strnlen(s, maxlen);
	ipc_write(s, len);
	ipc_put8(0);
}

pid_t _spawn(const char *path, char *const *argv, char *const *envp, const struct spawnattr *sat) /* sat unused for now */
{
	ipc_rewindw(_MM_spawn);
	ipc_putstr(path, MAX_PATH);
	char *const *p;
	for (p = argv; *p; p++)
		ipc_putstr(*p, MAX_PERARG);
	ipc_put8(0);
	for (p = envp; *p; p++)
		ipc_putstr(*p, MAX_PERENV);
	ipc_put8(0);
	ipc_call(SVFD_MM);
	return ipc_getw();
}
