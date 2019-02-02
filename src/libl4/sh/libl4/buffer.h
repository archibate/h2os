#pragma once

#include <l4/types.h>

int l4Buffer_WriteFrags(l4CPtr_t cptr, const void *buf, size_t size, int rewind);
