#pragma once

#include <l4/thread.h>
#include <l4/capability.h>

int do_TCB_SetCap(tcb_t *tcb, word_t cidx, cap_t *cap);
int do_TCB_GetCap(tcb_t *tcb, word_t cidx, cap_t *capDest);
