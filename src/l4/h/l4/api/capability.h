#pragma once

#include <l4/object/capability.h>

int cap_action(struct capability *cap, unsigned int action, unsigned long argument);
int cap_send(struct capability *cap, unsigned long addr, const char *buf, size_t size);
int cap_recv(struct capability *cap, unsigned long addr, char *buf, size_t size);
