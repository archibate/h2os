// https://github.com/archibate/OS67/blob/master/drv/kb.c
#include <h4/sys/types.h>
#include <h4/sys/ipc.h>
#include <h4/servers.h>
#include <h4/file/sysnr.h>
#include <memory.h>
#include <errno.h>
#include "ide.h"
#include <bug.h>

static int ide;

void ide_init(void)
{
	ide = ipc_open(SVID_IDEDRV, IPC_CREAT|IPC_SERVER);
	BUG_ON(ide < 0);
}

#define CLMAX(x, max) do { if ((x) >= (max)) (x) = (max); } while (0)

static dev_t ide_dev = 0;
char ide_buf[BSIZE];
static blkno_t ide_blkno = -1;

ssize_t ide_pread(void *buf, size_t len, off_t off)
{
	blkno_t blkno = off / BSIZE;
	size_t skip = off % BSIZE;
	size_t rlen = len;
	CLMAX(rlen, BSIZE - skip);

	if (ide_blkno != blkno) {
		ide_rdblk(ide_dev, blkno, &ide_buf);
	}
	ide_blkno = blkno++;
	memcpy(buf, ide_buf + skip, rlen);
	buf += rlen;

	while (rlen + BSIZE < len) {
		ide_rdblk(ide_dev, blkno++, buf);
		buf += BSIZE;
		rlen += BSIZE;
	}

	size_t rest = len - rlen;
	if (rest != 0) {
		ide_rdblk(ide_dev, blkno, &ide_buf);
		memcpy(buf, ide_buf, rest);
		ide_blkno = blkno++;
	}
	return len;
}

ssize_t ide_pwrite(const void *buf, size_t len, off_t off)
{
	blkno_t blkno = off / BSIZE;
	size_t skip = off % BSIZE;
	size_t rlen = len;
	CLMAX(rlen, BSIZE - skip);

	if (ide_blkno != blkno && !(skip == 0 || rlen == BSIZE)) {
		ide_rdblk(ide_dev, blkno, &ide_buf);
	}
	ide_blkno = blkno;
	memcpy(ide_buf + skip, buf, rlen);
	ide_wrblk(ide_dev, blkno++, &ide_buf);
	buf += rlen;

	while (rlen + BSIZE < len) {
		ide_wrblk(ide_dev, blkno++, buf);
		buf += BSIZE;
		rlen += BSIZE;
	}

	size_t rest = len - rlen;
	if (rest != 0) {
		if (rlen != BSIZE) {
			ide_rdblk(ide_dev, blkno, &ide_buf);
		}
		memcpy(ide_buf, buf, rest);
		ide_wrblk(ide_dev, blkno, &ide_buf);
		ide_blkno = blkno++;
	}
	return len;
}

void ide_serve_ipc(void)
{
	ipc_recv(ide);
	unsigned int nr = ipc_getw();
	switch (nr) {

	case _FILE_pread:
	{
		size_t len = ipc_getw();
		off_t off = ipc_getw();
		ipc_seek_set(sizeof(ssize_t));
		void *buf = ipc_getbuf(&len);
		ssize_t ret = ide_pread(buf, len, off);
		ipc_rewindw(ret);
	} break;

	case _FILE_pwrite:
	{
		size_t len = ipc_getw();
		off_t off = ipc_getw();
		const void *buf = ipc_getbuf(&len);
		ssize_t ret = ide_pwrite(buf, len, off);
		ipc_rewindw(ret);
	} break;

	default:
		ipc_putw(-ENOTSUP);
	}
	ipc_reply();
}

int main(void)
{
	ide_init();
	while (1) {
		ide_serve_ipc();
	}
}

