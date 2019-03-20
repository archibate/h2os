#pragma once

#include "buf.h"

void bcache_init(void);
struct buf *bread(blkno_t blkno);
void bwrite(struct buf *b);
void brelse(struct buf *b);
