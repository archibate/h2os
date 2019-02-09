#pragma once

#include <l4/types.h>


int l4TCB_SetPriority(l4CPtr_t cptr, l4Byte_t priority);
int l4TCB_Active(l4CPtr_t cptr);
int l4TCB_Suspend(l4CPtr_t cptr);
