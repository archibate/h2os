#pragma once

#include <l4/types.h>

void l4MsgInit(void);
size_t l4MsgGetBufSize(void);
l4Ret_t l4MsgWrite(const void *buf, size_t size);
l4Ret_t l4MsgCapWrite(const l4CPtr_t *caps, size_t capnr);
l4Ret_t l4MsgRead(void *buf, size_t size);
l4Ret_t l4MsgInvoke(l4CPtr_t cptr);
