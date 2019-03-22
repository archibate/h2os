#pragma once

#include <bug.h>

#ifndef FIFO_MAX
#define FIFO_MAX 256
#endif

struct fifo
{
	int rd, wr;
	char buf[FIFO_MAX];
};

static void fifo_init(struct fifo *f)
{
	f->rd = f->wr = 0;
}

static void fifo_put(struct fifo *f, char c)
{
	f->buf[f->wr] = c;
	f->wr = (f->wr + 1) % sizeof(f->buf);
}

static char fifo_get(struct fifo *f)
{
	char c = f->buf[f->rd];
	f->rd = (f->rd + 1) % sizeof(f->buf);
	return c;
}

static char fifo_peek(struct fifo *f)
{
	return f->buf[f->rd];
}

static int fifo_rdmax(struct fifo *f)
{
	return (f->wr - f->rd) % sizeof(f->buf);
}

static int fifo_wrmax(struct fifo *f)
{
	return sizeof(f->buf) - 1 - fifo_rdmax(f);
}

static int fifo_empty(struct fifo *f)
{
	return f->wr == f->rd;
}

static int fifo_full(struct fifo *f)
{
	return fifo_wrmax(f) == 0;
}
