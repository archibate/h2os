#pragma once

#include <l4/object/capability.h>

int cap_endpoint_send(struct capability *cap, unsigned long addr, const char *buf, size_t size);
int cap_endpoint_recv(struct capability *cap, unsigned long addr, char *buf, size_t size);
