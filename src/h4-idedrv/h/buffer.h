#pragma once

#define SECSIZE 512
#define BLKSIZE 1024
#define SPB     (BLKSIZE/SECSIZE)

/* Block Status */
#define B_VALID (1<<0) // has been read from disk
#define B_DIRTY (1<<1) // need to be written to disk
#define B_BUSY  (1<<2) // buffer busy (not bwritten)

#include <list.h>

typedef unsigned int blkno_t;
typedef unsigned char dev_t;

struct buf
{
	unsigned char flags;
	dev_t dev;
	blkno_t blkno;
	struct hlist_node hlist;
	char data[BLKSIZE];
};

struct buf *bget(dev_t dev, blkno_t blkno);
struct buf *bread(dev_t dev, blkno_t blkno);
bool bwrite(struct buf *b);
void brelse(struct buf *b);

// IPAs
bool buf_onrw(struct buf *b);
