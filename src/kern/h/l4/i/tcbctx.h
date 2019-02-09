#pragma once

#include <l4/thread.h>

int do_TCB_SetPCSP(tcb_t *tcb, word_t pc, word_t sp);
