#pragma once

#include <l4/object/endpoint.h>
#include <l4/object/fdentry.h>

int endp_recv(struct endpoint *ep, bool block);
int endp_call(struct fd_entry *fde, bool block, bool recv, int phase);
int endp_reply(uintptr_t badge, uintptr_t offset);
