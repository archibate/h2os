#pragma once

#include <l4/types.h>

l4Ret_t l4Buffer_WriteFrags(l4CPtr_t cptr, const void *buf, size_t size, int rewind);
l4Ret_t l4Buffer_ReadFrags(l4CPtr_t cptr, void *buf, size_t size, int rewind);
