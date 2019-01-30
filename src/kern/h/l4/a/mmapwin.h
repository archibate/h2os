#pragma once

#include <l4/memory.h>

void *mmapwin(mem_t *mem, word_t addr, int rw);
