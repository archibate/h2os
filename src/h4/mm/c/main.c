#include <h4/sys/types.h>
#include <h4/sys/ipc.h>
#include <l4/api/mmctl.h>
#include <h4/mm/sysnr.h>
#include <h4/servers.h>
#include <h4/mm/defines.h>
#include <c4/liballoc.h>
#include <h4/fs.h>
#include <errno.h>
#include <malloc.h>
#include <case.h>
#include <printk.h>
#include <stddef.h>
#include <bug.h>
#include "loader.h"
#include "passarg.h"

int mm_execve(int mmc, const char *path, char *const *argv, char *const *envp)
{
#if 0
	printk("mm_execve:");
	printk("  path: %s", path);
	char *const *p;
	printk("  argv:");
	for (p = argv; *p; p++)
		printk("    %s", *p);
	printk("  envp:");
	for (p = envp; *p; p++)
		printk("    %s", *p);
#endif

	int fd = open(path, O_RDONLY);
	if (fd < 0)
		return fd;
	uintptr_t pc, sp;
	int ret = loadelf(mmc, fd, &pc);
	close(fd);
	if (ret < 0)
		return ret;
	stack_init(mmc, argv, envp, &sp);
	sys_mmctl_setpcsp(mmc, pc, sp);
	return 0;
}

static char *ipc_getstr(size_t maxlen)
{
	char *q = ipc_getbuf(NULL);
	size_t len = strnlen(q, maxlen);
	if (len == 0) {
		ipc_seek_cur(1);
		return NULL;
	}
	ipc_seek_cur(!q[len] ? len+1 : len);
	char *p = malloc(len+1);
	memcpy(p, q, len);
	p[len] = 0;
	return p;
}

static void ipc_getstrarr(char *arr[], size_t maxlen, size_t maxcnt)
{
	int i;
	for (i = 0; i < maxcnt; i++) {
		arr[i] = ipc_getstr(maxlen);
		//printk("%p", arr[i]);
		if (arr[i] == NULL)
			return;
	}
	arr[maxcnt] = NULL;
}

const int libh4_serve_id = SVID_MM;

int ipc_onexecve(void)
{
	char *path = ipc_getstr(MAX_PATH);
	if (!path)
		return -EINVAL;
	char *argv[MAX_ARGV+1];
	ipc_getstrarr(argv, MAX_PERARG, MAX_ARGV);
	char *envp[MAX_ENVP+1];
	ipc_getstrarr(envp, MAX_PERENV, MAX_ENVP);
	int ret = mm_execve(-1, path, argv, envp);
	free(path);
	char **p;
	for (p = argv; *p; p++)
		free(*p);
	for (p = envp; *p; p++)
		free(*p);
	return ret;
}

int main(void)
{
	static char buffer[4096*32];
	liballoc_set_memory(buffer, sizeof(buffer));

	while (1) {
		ipc_recv();
		int nr = ipc_getw();
		switch (nr) {
		CASE(_MM_execve) {
			int ret = ipc_onexecve();
			ipc_rewindw(ret);
		}
		DEFAULT {
			ipc_rewindw(-ENOTSUP);
		}
		}
		ipc_reply();
	}
}
