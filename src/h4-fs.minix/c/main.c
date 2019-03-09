#include <l4/types.h>
#include <l4/api/sched.h>
#include <h4/sys/types.h>
#include <h4/sys/ipc.h>
#include <h4/servers.h>
#include <printk.h>
#include "minix.h"
#include "bug.h"

static int fs, hd;

void fs_init(void)
{
	fs = ipc_open(SVID_ROOTFS, IPC_CREAT | IPC_RECV);
	BUG_ON(fs < 0);
}

void hd_init(void)
{
	hd = ipc_open(SVID_IDEHD, IPC_CREAT | IPC_CLIENT);
	BUG_ON(hd < 0);
}

void main(void)
{
	hd_init();
	fs_init();

	struct super_block sb;
	read_sb(hd, &sb);

	sys_exit();
}
