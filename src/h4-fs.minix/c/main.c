#include <l4/types.h>
#include <l4/api/sched.h>
#include <h4/sys/types.h>
#include <h4/sys/ipc.h>
#include <h4/servers.h>
#include <h4/proc.h>
#include <printk.h>
#include <bug.h>
#include "minix.h"
#include "bcache.h"
#include "hd.h"

static int fs;

void fs_server_init(void)
{
	fs = ipc_open(SVID_ROOTFS, IPC_CREAT | IPC_SERVER);
	BUG_ON(fs < 0);
}

void main(void)
{
	fs_server_init();
	hd_init();
	bcache_init();

	_exit(0);
}
