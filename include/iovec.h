#pragma once

#include <inttypes.h>

struct iovec
{
	void *iov_base;
	size_t iov_len;
};
