// https://github.com/archibate/OS67/blob/master/fs/sb.c
#include "minix.h"
#include "sb.h"
#include "bcache.h"
#include <h4/blk/api.h>
#include <printk.h>
#include <memory.h>

static void print_sb(struct super_block *sb)
{
	printk("super block:");
	printk("inodes %d", sb->ninodes);
	printk("zones %d", sb->nzones);
	printk("inode map blocks %d", sb->imap_blk);
	printk("zone map blocks %d", sb->zmap_blk);
	printk("first data zone %d", sb->fst_data_zone);
	printk("zone size %d", 1024 << sb->log_zone_size);
	printk("max file size %d", sb->max_size);
	printk("magic number %#x", sb->magic);
	printk("state %d", sb->state);
}

void read_sb(struct super_block *sb)
{
	struct buf *bp;
	bp = bread(1);

	memcpy(sb, bp->data, sizeof(*sb));
	brelse(bp);
	print_sb(sb);
}
