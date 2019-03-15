#include "minix.h"
#include "bcache.h"
#include "bitmap.h" 
#include "sb.h"
#include <assert.h>
#include <printk.h>

#define BPB (BSIZE*8)

/* clear a block */
static void bzero(uint32_t blkno)
{
	struct buf *bp;

	printk("bzero: zero blk-%d\n",blkno);

	bp = bread(blkno);
	memset(bp->data, 0, BSIZE);
	bwrite(bp);
	brelse(bp);
}

/* alloc a zeroed data block  */
int balloc(void)
{
	uint32_t b, bi, m;
	struct buf *bp;
	struct super_block sb;
	uint32_t v_blkno;

	//bp = 0;
	read_sb(&sb);
	for (b = 0; b < sb.nzones; b += BPB) {
		bp = bread(ZMAP_BLK(sb, b));
		for(bi = 0; bi < BPB && b + bi < sb.nzones; bi++) {
			m = 1 << (bi%8);
			if ((bp->data[bi/8] & m) == 0) { // This block is free
				v_blkno = sb.fst_data_zone + b + bi - 1;
				printk("balloc: alloc blk-%d zmap blk-%d bi: %d\n", v_blkno, ZMAP_BLK(sb, b), bi);

				bp->data[bi/8] |= m;
				bwrite(bp);                 // mark this block as uesed in bitmap
				brelse(bp);
				bzero(v_blkno);         // clear block with zero
				return v_blkno;
			}
		}
		brelse(bp);
	}
	printk(KL_ERROR "balloc: out of blocks");
	return -1;
}

void bfree(blkno_t blkno)
{
	struct buf *bp;
	struct super_block sb;
	uint32_t bi, m;

	read_sb(&sb);

	assert_info(blkno >= sb.fst_data_zone ,"bfree: free a non-data block");
	assert_info(blkno < sb.nzones, "bfree: block number out of range");

	blkno -= sb.fst_data_zone - 1;

	bp = bread(ZMAP_BLK(sb, blkno));  // 找到该block对应的bitmap所在的block

	bi = blkno%BPB;
	m = 1 << (bi%8);

	printk("bfree: free blk-%d zmapblk-%d bi: %d\n", blkno, ZMAP_BLK(sb, blkno - sb.fst_data_zone), bi);
	assert_info(bp->data[bi/8] & m, "bfree: freeing a free block");

	bp->data[bi/8] &= ~m;
	bwrite(bp);
	brelse(bp);
}

/* alloc a zeroed inode 
 * NB: inode number is start at 1
 */
int _ialloc(void)
{
	uint32_t b, bi, m;
	ino_t ino;
	struct buf *bp;
	struct d_inode *d_ip;
	struct super_block sb;

	read_sb(&sb);

	for (b = 1; b < sb.ninodes; b += IPB) {
		bp = bread(IMAP_BLK(sb, b));
		for (bi = 0; bi < IPB && bi < sb.ninodes; bi++) {
			m = 1 << (bi%8);
			if ((bp->data[bi/8] & m) == 0) {

				ino = b + bi - 1;
				printk("_ialloc: alloc inode-%d\n", ino);

				bp->data[bi/8] |= m;
				bwrite(bp);
				brelse(bp);


				/* fill inode with zero */
				bp = bread(IBLK(sb,ino));
				d_ip = (struct d_inode *)bp->data + (ino - 1)%IPB;
				memset(d_ip, 0, sizeof(*d_ip)); bwrite(bp); brelse(bp);

				return ino;
			}
		}
	}
	printk(KL_ERROR "_ialloc: out of inode");
	return -1;
}

void _ifree(ino_t ino)
{
	uint32_t bi, m;
	struct buf *bp;
	struct super_block sb;

	printk("_ifree: free inode-%d\n", ino);

	read_sb(&sb);

	assert_info(ino > 0, "_ifree: inode-0 is no used");
	assert_info(ino < sb.ninodes, "_ifree: inode number out of range");

	bp = bread(IMAP_BLK(sb, ino));
	bi = ino/8;
	m = 1 << (ino%8);

	assert_info(bp->data[bi] & m, "_ifree: free a non-free inode");
	bp->data[bi] &= ~m;
	bwrite(bp);
	brelse(bp);
}
