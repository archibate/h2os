#pragma once

#include <l4/memory.h>

void *mmappage(mem_t *mem, word_t addr, int rw);
