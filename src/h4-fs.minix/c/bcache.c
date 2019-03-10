// https://raw.githubusercontent.com/archibate/OS67/master/fs/bcache.c
#include "minix.h"
#include "bcache.h"
#include "hd.h"
#include <printk.h>
#include <assert.h>
#include <stddef.h>

#define NBUF 16

static struct bcache
{
	struct buf buf[NBUF];
	/* head of MRU(Most Recently Used) List, head.next is recently used */
	struct buf head;
} bcache;

/* create linked list(MRU Cache) of buffer, in reverse */
void bcache_init(void)
{
	struct buf *b;
	bcache.head.prev = &bcache.head;
	bcache.head.next = &bcache.head;
	for (b = bcache.buf; b < bcache.buf + NBUF; b++) {
		b->next = bcache.head.next;
		b->prev = &bcache.head;
		b->blkno = -1;
		bcache.head.next->prev = b;
		bcache.head.next = b;
	}
}

/* look up a NON_BUSY buffer cache by blkno
 * if found set B_BUSY and return
 * else alloc a new buffer
 */
static struct buf *bget(blkno_t blkno)
{
	printk("bget: try to get blk-%d", blkno);
	struct buf *b;
loop:
	/* is the sector already cached? */
	for (b = bcache.head.next; b != &bcache.head; b = b->next) {
		if (b->blkno == blkno) {    // found require buffer
			if (!(b->flags & B_BUSY)) {
				b->flags |= B_BUSY;
				return b;
			}
			// if buffer found but B_BUSY, wait and lookup again
			//panic("bget: buffer busy(TEMP)");
			// 这里可能会死锁...
			goto loop;
		}
	}

	printk("bget: blk-%d not cached, try to find a new one", blkno);
	/* not cached: recycle some non-busy and clean buffer */
	for (b = bcache.head.prev; b != &bcache.head; b = b->prev) {
		if ((b->flags & B_BUSY) == 0 && (b->flags & B_DIRTY) == 0) {
			b->blkno = blkno;
			b->flags = B_BUSY;
			return b;
		}
	}
	printk(KL_ERROR "bget: out of buffer");
	return NULL;
}

/* return a B_BUSY buffer with the content of indicated disk sector */
struct buf *bread(blkno_t blkno)
{
	printk("bread: read blk-%d", blkno);
	struct buf *b;
	b = bget(blkno);
	if (!(b->flags & B_VALID)) {
		hd_rw(b);
	}
	return b;
}

void bwrite(struct buf *b)
{
	printk("bwrite: wirte blk-%d",b->blkno);
	assert_info(b->flags & B_BUSY, "bwrite: buffer no busy");
	b->flags |= B_DIRTY;
	hd_rw(b);
}

/* release a B_BUSY buffer, mov it to the haed of MRU list */
void brelse(struct buf *b)
{
	printk("brelse: relse blk-%d", b->blkno);
	assert_info(b->flags & B_BUSY, "brelse: buffer no busy");

	b->next->prev = b->prev;
	b->prev->next = b->next;
	b->next = bcache.head.next;
	b->prev = &bcache.head;
	bcache.head.next->prev = b;
	bcache.head.next = b;

	b->flags &= ~B_BUSY;
}
