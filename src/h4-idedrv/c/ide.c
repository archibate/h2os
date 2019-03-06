/* https://github.com/archibate/OS67/blob/master/drv/ide.c */
#include <inttypes.h>
#include <l4/machine/asm/io.h>
#include <h4/misc/bug.h>
#include "ideports.h"

static bool ide_wait(void)
{
	int timeout = 20000;
	int r;

	while ((r = inb(IDE_STAT)) & IDE_BSY) {
		BUG_ON(timeout-- <= 0);
	}

	return (r & (IDE_DF|IDE_ERR)) == 0;
}

static bool ide_seek(dev_t dev, blkno_t blkno)
{
	if (!ide_wait())
		return false;

	uint32_t lba = blkno * SPB;

	outb(IDE_SECTNR, SPB);

	outb(IDE_LBA0, lba         & 0xff);
	outb(IDE_LBA1, (lba >> 8)  & 0xff);
	outb(IDE_LBA2, (lba >> 16) & 0xff);

	uint8_t cur = 0xe0;
	cur |= (dev & 0x1) << 4;
	cur |= (lba >> 24) & 0x0f;
	outb(IDE_CURR, cur);
}

static void ide_wrblk(const char *buf)
{
	outb(IDE_CMD, IDE_CMD_WRITE);
	outsl(IDE_DAT, buf, BLKSIZE/4);
}

static void ide_rdblk(char *buf)
{
	outb(IDE_CMD, IDE_CMD_READ);
	insl(IDE_DAT, buf, BLKSIZE/4);
}

static char bcache[BLKSIZE];
static blkno_t bc_blkno;
static blkno_t bc_dev;
struct buf *buf;

ssize_t ide_serv_bsread1(dev_t dev, blkno_t blkno,
		char *buf, size_t size, size_t skip)
{
	if (buf->blkno == blkno && buf->dev == dev) {
		memcpy(buf, bcache, BLKSIZE);
	}
}
