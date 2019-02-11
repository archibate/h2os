#pragma once

#include <l4/types.h>
#include <l4/capability.h>

int sysInvoke(cap_t *target, cap_t *capDest, word_t *shortMsg, word_t *extraMsg);
