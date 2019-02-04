#pragma once

#include <l4/capability.h>
#include <l4/thread.h>

void init_mycaps(cap_t *cspace, tcb_t *tcb);
