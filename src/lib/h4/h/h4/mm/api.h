#pragma once

#include <inttypes.h>

void *sbrk(ptrdiff_t incptr);
void *brk(void *p);
