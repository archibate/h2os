// https://github.com/archibate/OS67/blob/master/drv/ide.c
#include "minix.h"
#include "buf.h"
#include "hd.h"
#include <printk.h>
#include <assert.h>
#include <h4/sys/types.h>
#include <h4/sys/ipc.h>
#include <h4/servers.h>
#include <h4/file/api.h>

static int hd;

void hd_init(void)
{
	hd = ipc_open(SVID_IDEHD, IPC_CREAT | IPC_CLIENT);
	BUG_ON(hd < 0);
}

void hd_rw(struct buf *b)
{
	assert_info((b->flags & B_BUSY), "hd_rw: buf not busy");
	assert_info((b->flags & (B_VALID|B_DIRTY)) != B_VALID, "hd_rw: nothing to do");

	pread(hd, &b->data, BSIZE, b->blkno*BSIZE);
}
