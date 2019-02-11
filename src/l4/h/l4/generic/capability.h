#pragma once

#include <l4/object/capability.h>

void cap_init(struct capability *cap);
int cap_consume(struct capability *cap, unsigned long quantity);
int cap_free(struct capability *cap, unsigned long quantity);
int cap_limit(struct capability *cap, unsigned long base, unsigned long size);
int cap_verify(struct capability const *cap, unsigned long addr);
unsigned long cap_address(struct capability const *cap, unsigned long addr);
