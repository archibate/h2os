#pragma once

#include <h4/file/api.h>
#include <h4/blk/defines.h>

#define blkread(hd, buf, cnt, bno)  pread(hd, buf, (cnt) * BSIZE, (bno) * BSIZE)
#define blkwrite(hd, buf, cnt, bno) pwrite(hd, buf, (cnt) * BSIZE, (bno) * BSIZE)
