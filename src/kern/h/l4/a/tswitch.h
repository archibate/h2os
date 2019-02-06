#pragma once

#include <l4/thread.h>

void switch_task(tcb_t *oldTcb, tcb_t *newTcb);
