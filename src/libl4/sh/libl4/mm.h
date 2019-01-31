#pragma once

#include <l4/types.h>

int l4Split(l4CPtr_t cptr, l4Word_t point);
int l4Allocate(l4CPtr_t cptr, l4Word_t num, l4Word_t retype);
