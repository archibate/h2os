#pragma once

#include <inttypes.h>

void stack_init(int mmc, char *const *argv, char *const *envp, uintptr_t *sp);
