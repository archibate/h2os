#pragma once

#include <l4/object/asyncep.h>

struct ktcb;

void async_init(struct async_ep *aep);
void async_revoke(struct async_ep *aep);
void async_delete(struct async_ep *aep);
void async_pulse(struct async_ep *aep);
bool async_poll(struct async_ep *aep);
void async_listen(struct async_ep *aep, struct ktcb *listener);
