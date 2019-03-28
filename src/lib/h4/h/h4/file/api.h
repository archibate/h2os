#pragma once

#include <h4/file/types.h>

ssize_t pread(int fd, void *buf, size_t len, off_t off);
ssize_t pwrite(int fd, const void *buf, size_t len, off_t off);
ssize_t read(int fd, void *buf, size_t len);
ssize_t write(int fd, const void *buf, size_t len);
off_t lseek(int fd, off_t off, int whence);
