/* https://github.com/archibate/OS67/blob/master/drv/ide.c */
#include <inttypes.h>
#include <l4/machine/asm/io.h>
#include "ide.h"
#include "ideports.h"
#include <bug.h>

static int ide_wait(void)
{
	int timeout = 20000;
	int r;

	while ((r = inb(IDE_STAT)) & IDE_BSY) {
		BUG_ON(timeout-- <= 0);
	}

	return (r & (IDE_DF|IDE_ERR)) == 0;
}

static void ide_seek(dev_t dev, blkno_t blkno)
{
	BUG_ON(!ide_wait());

	uint32_t lba = blkno * PBPB;

	outb(IDE_SECTNR, PBPB);

	outb(IDE_LBA0, lba         & 0xff);
	outb(IDE_LBA1, (lba >> 8)  & 0xff);
	outb(IDE_LBA2, (lba >> 16) & 0xff);

	uint8_t cur = 0xe0;
	cur |= (dev & 0x1) << 4;
	cur |= (lba >> 24) & 0x0f;
	outb(IDE_CURR, cur);
}

void ide_wrblk(dev_t dev, blkno_t blkno, const void *buf)
{
	ide_seek(dev, blkno);

	outb(IDE_CMD, IDE_CMD_WRITE);
	outsl(IDE_DAT, buf, BSIZE/4);
}

void ide_rdblk(dev_t dev, blkno_t blkno, void *buf)
{
	ide_seek(dev, blkno);

	outb(IDE_CMD, IDE_CMD_READ);
	insl(IDE_DAT, buf, BSIZE/4);
}
