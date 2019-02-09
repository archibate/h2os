#pragma once

#include <l4/types.h>

int l4TCB_SetCap(l4CPtr_t cptr, l4Word_t cidx, l4CPtr_t cap);
int l4TCB_GetCap(l4CPtr_t cptr, l4Word_t cidx);
