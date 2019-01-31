#pragma once

#include <l4/types.h>

size_t phyread(word_t paddr, char *buf, size_t len);
size_t phywrite(word_t paddr, const char *buf, size_t len);
void *phymmap(word_t paddr, size_t len, int rw);
void phyunmap(void *p);
