#pragma once

#include <inttypes.h>

ssize_t pread(int fd, void *buf, size_t len, off_t off);
ssize_t pwrite(int fd, const void *buf, size_t len, off_t off);
