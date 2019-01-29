#pragma once

#include <l4/types.h>
#include <l4/thread.h>
#include <l4/capability.h>

int sysInvoke(cap_t *target, word_t *tag);
int sysSubInvoke(cap_t *root, cptr_t cptr, bits_t depth, word_t *tag);
