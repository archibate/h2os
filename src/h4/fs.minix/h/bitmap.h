#pragma once

int balloc(void);
void bfree(blkno_t blkno);
int _ialloc(void);
void _ifree(ino_t ino);
