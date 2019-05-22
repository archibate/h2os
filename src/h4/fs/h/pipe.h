#pragma once

#define PIPSIZ 2048

struct pipe {
	uint32_t rd, wr;
	char buf[PIPSIZ];
};

ssize_t pipe_write(
