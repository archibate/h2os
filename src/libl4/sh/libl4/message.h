#pragma once

#include <l4/types.h>

void l4MsgInit(void);
size_t l4MsgWrite(const void *buf, size_t size);
l4Ret_t l4MsgInvoke(l4CPtr_t cptr);
