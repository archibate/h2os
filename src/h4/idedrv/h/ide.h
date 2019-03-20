#pragma once

#include <h4/blk/defines.h>
#include <h4/blk/types.h>

#define PBSIZE  512
#define PBPB    (BSIZE/PBSIZE)

void ide_wrblk(dev_t dev, blkno_t blkno, const void *buf);
void ide_rdblk(dev_t dev, blkno_t blkno, void *buf);
