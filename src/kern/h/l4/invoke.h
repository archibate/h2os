#pragma once

#include <l4/types.h>
#include <l4/thread.h>
#include <l4/capability.h>

int sysInvoke(cap_t *target, tcb_t *sender, word_t *tag);
