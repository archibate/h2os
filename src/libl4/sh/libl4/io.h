#pragma once

#include <l4/types.h>

ssize_t l4PWrite(l4CPtr_t cptr, const void *data, size_t size, off_t off);
ssize_t l4PRead(l4CPtr_t cptr, void *data, size_t size, off_t off);
ssize_t l4Write(l4CPtr_t cptr, const void *data, size_t size);
ssize_t l4Read(l4CPtr_t cptr, void *data, size_t size);
