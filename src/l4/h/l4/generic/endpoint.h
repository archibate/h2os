#pragma once

#include <l4/object/endpoint.h>

struct ktcb;

void endpoint_init(struct endpoint *ep);
void endpoint_revoke(struct endpoint *ep);
void endpoint_delete(struct endpoint *ep);
void endpoint_call(struct endpoint *ep, struct ktcb *caller, bool block, bool recv);
void endpoint_wait(struct endpoint *ep, struct ktcb *waiter);
