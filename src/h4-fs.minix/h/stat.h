#pragma once

#include "statmode.h"

struct stat
{
	dev_t dev;
	ino_t ino;
	mode_t mode;
	size_t size;
};
