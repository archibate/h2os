#pragma once

#include <l4/thread.h>

void Arch_switchTask(tcb_t *oldTcb, tcb_t *newTcb);
