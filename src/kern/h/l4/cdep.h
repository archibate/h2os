#pragma once

#include <l4/capability.h>

void cdepend(cap_t *child, cap_t *parent);
void cdelete(cap_t *cap);
