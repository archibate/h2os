#pragma once

#include <l4/types.h>

ssize_t l4Read(l4CPtr_t cptr, void *data, l4Word_t size);
ssize_t l4Write(l4CPtr_t cptr, const void *data, l4Word_t size);
