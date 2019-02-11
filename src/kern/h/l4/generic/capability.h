#pragma once

#include <l4/api/capability.h>

void cap_init(struct capability *cap);
int cap_consume(struct capability *cap, unsigned long quantity);
int cap_free(struct capability *cap, unsigned long quantity);
int cap_verify(struct capability const *cap, unsigned long addr);
unsigned long cap_address(struct capability const *cap, unsigned long addr);
int cap_limit(struct capability *cap, unsigned long base, unsigned long size);
int cap_action(struct capability *cap, unsigned int action, unsigned long argument);
int cap_send(struct capability *cap, unsigned long addr, const char *buf, size_t size);
int cap_recv(struct capability *cap, unsigned long addr, char *buf, size_t size);
