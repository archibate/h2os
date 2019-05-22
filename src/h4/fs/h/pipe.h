#pragma once

#define PIPSIZ 2048

struct pipe {
	uint32_t rd, wr;
	char buf[PIPSIZ];
};

static struct pipe *new_pipe(void)
{
	return zalloc(sizeof(struct pipe));
}

static bool pipe_empty(struct pipe *pip)
{
	return pip->rd == pip->wr;
}

static bool pipe_full(struct pipe *pip)
{
	return (pip->rd - pip->wr) % PIPSIZ == 1;
}

static void pipe_put(struct pipe *pip, char data)
{
	//BUG_ON(pipe_full(pip));
	pip->buf[pip->wr] = data;
	pip->wr = (pip->wr + 1) % PIPSIZ;
}

static char pipe_get(struct pipe *pip)
{
	//BUG_ON(pipe_empty(pip));
	char data = pip->buf[pip->rd];
	pip->rd = (pip->rd + 1) % PIPSIZ;
	return data;
}

static ssize_t pipe_read(struct pipe *pip, char *buf, size_t len)
{
	if (pipe_empty(pip))
		return -EAGAIN;

	int n;
	for (n = 0; n < len && !pipe_empty(pip); n++)
		buf[n] = pipe_get(pip);

	return n;
}

static ssize_t pipe_write(struct pipe *pip, const char *buf, size_t len)
{
	if (pipe_full(pip))
		return -EAGAIN;

	int n;
	for (n = 0; n < len && !pipe_full(pip); n++)
		pipe_put(pip, buf[n]);

	return n;
}
