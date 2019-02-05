#pragma once

#include <l4/types.h>
#include <l4/asm/context.h>

typedef l4Word_t l4ThreadContext_t[L4_ContextWords];
int l4TCB_SetContext(l4CPtr_t cptr, l4ThreadContext_t const *context);
int l4TCB_SetCap(l4CPtr_t cptr, l4Word_t cidx, l4CPtr_t cap);
int l4TCB_GetCap(l4CPtr_t cptr, l4Word_t cidx);
int l4TCB_Active(l4CPtr_t cptr);
