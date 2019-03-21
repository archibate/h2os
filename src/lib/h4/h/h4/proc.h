#pragma once

#include <compiler.h>
#include <h4/sys/types.h>

void _NORETURN _exit(int status);
int pause(void);
pid_t getpid(void);
