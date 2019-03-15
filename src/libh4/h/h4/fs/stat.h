#pragma once

#include <h4/fs/stat-bits.h>

struct stat
{
	dev_t dev;
	ino_t ino;
	mode_t mode;
	size_t size;
};
