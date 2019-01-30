#pragma once

#include <l4/fpage.h>

void *fpgmapwin(fpage_t *fpage, word_t offset, int rw);
