#pragma once

#include <compiler.h>

void _NORETURN __crt_start(void *ebss, int (*main)(int, char *const *, char *const *), void *sp);
