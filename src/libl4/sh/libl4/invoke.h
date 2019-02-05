#pragma once

#include <l4/types.h>

l4Ret_t l4Invoke(l4CPtr_t cptr, const void *buf, size_t size);
#if 0
l4Ret_t l4CapInvoke(l4CPtr_t cptr, const void *buf, size_t size,
		    const l4CPtr_t *caps, size_t capnr);
#endif
