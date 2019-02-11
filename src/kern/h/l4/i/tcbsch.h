#pragma once

#include <l4/i/tcb.h>

int do_TCB_SetPriority(tcb_t *tcb, byte_t prio);
int do_TCB_Active(tcb_t *tcb);
int do_TCB_Suspend(tcb_t *tcb);
