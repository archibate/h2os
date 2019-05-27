#include <h4/sys/types.h>
#include <h4/sys/spawn.h>
#include <h4/sys/ipc.h>
#include <l4/api/mmctl.h>
#include <h4/mm/sysnr.h>
#include <h4/servers.h>
#include <h4/sys/spawn.h>
#include <h4/filedes.h>
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

#if 0
void *hook_malloc(size_t size)
{
	printk("mm: hook_malloc(%d)", size);
	return malloc(size);
}
#define malloc hook_malloc
#endif

int mm_spawn(int src_mmc, const char *path, char *const *argv, char *const *envp, struct spawnattr *sat)
{
#if 0
	printk("mm_spawn:");
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
	//if (fd < 0) printk("fd=%d", fd);
	if (fd < 0)
		return fd;
	uintptr_t pc, sp;
	int mmc = sys_create_mm(src_mmc);
	//if (mmc < 0) printk("mmc=%d", mmc);
	if (mmc < 0)
		return mmc;
	int ret = loadelf(mmc, fd, &pc);
	close(fd);
	//if (ret < 0) printk("ret=%d", ret);
	if (ret < 0)
		return ret;
	stack_init(mmc, argv, envp, &sp);
	sys_mm_btw_fdup2(src_mmc, sat->stdio[0], mmc, 0);
	sys_mm_btw_fdup2(src_mmc, sat->stdio[1], mmc, 1);
	sys_mm_btw_fdup2(src_mmc, sat->stdio[2], mmc, 2);
	//if (sat->stdio[0] != 0 || sat->stdio[1] != 1)
	//printk("!!!%d!%d!%d!", sat->stdio[0], sat->stdio[1], sat->stdio[2]);
	sys_mm_btw_fdup2(src_mmc, SVFD_FS, mmc, SVFD_FS);
	sys_mm_btw_fdup2(src_mmc, SVFD_MM, mmc, SVFD_MM);
	BUG_ON(sys_mm_new_thread(mmc, pc, sp) < 0);
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

int ipc_onspawn(void)
{
	struct spawnattr sat;
	ipc_read(&sat, sizeof(sat));
	char *path = ipc_getstr(MAX_PATH);
	if (!path)
		return -EINVAL;
	char *argv[MAX_ARGV+1];
	ipc_getstrarr(argv, MAX_PERARG, MAX_ARGV);
	char *envp[MAX_ENVP+1];
	ipc_getstrarr(envp, MAX_PERENV, MAX_ENVP);
	int ret = mm_spawn(-1, path, argv, envp, &sat);
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
		CASE(_MM_spawn) {
			int ret = ipc_onspawn();
			ipc_rewindw(ret);
		}
		DEFAULT {
			ipc_rewindw(-ENOTSUP);
		}
		}
		ipc_reply();
	}
}
