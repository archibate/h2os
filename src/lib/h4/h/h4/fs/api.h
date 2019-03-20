#pragma once

#include <h4/sys/types.h>
#include <h4/fs/oflags.h>

int fs_open(int fs, const char *path, unsigned int flags);
int fs_openat(int fs, int fd, const char *path, unsigned int flags);
