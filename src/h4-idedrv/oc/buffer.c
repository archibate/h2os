#include "buffer.h"
#include <h4/misc/bug.h>

#define BCACHE_MAX 16

static struct buf bcache[BCACHE_MAX];
static struct hlist_head bchead;
static int bcerrno;

struct buf *bget(char dev, blkno_t blkno)
{
	bcerrno = 0;

	struct buf *b;
	hlist_for_each_entry2(b, bchead, hlist) {
		if (b->dev == dev && b->blkno == blkno) {
			if (b->flags & B_BUSY) {
				bcerrno = EBUSY;
				return NULL;
			}
			return b;
		}
	}

	/* not cached: recycle some non-busy and clean buffer */
	hlist_for_each_entry2(b, bchead, hlist) {
		if (!(b->flags & (B_BUSY|B_DIRTY))) {
			b->dev = dev;
			b->blkno = blkno;
			b->flags = B_BUSY;
			return b;
		}
	}

	bcerrno = ENOMEM;
	return NULL;
}

struct buf *bread(dev_t dev, blkno_t blkno)
{
	struct buf *b = bget(dev);
	if (!(b->flags & B_VALID)) {
		BUG_ON(!buf_onrw(b));
	}
	return b;
}

bool bwrite(struct buf *b)
{
	BUG_ON(!(b->flags & B_BUSY));

	hlist_add(&b->hlist, &bchead);
	b->flags &= ~B_BUSY;
	return buf_onrw(b);
}

void brelse(struct buf *b)
{
	BUG_ON(!(b->flags & B_BUSY));

	hlist_del(&b->hlist);
	hlist_add(&b->hlist, &bchead);

	b->flags &= ~B_BUSY;
}
