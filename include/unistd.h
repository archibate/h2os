#pragma once

#include <h4/sys/types.h>
#include <h4/sys/sched.h>
#include <h4/mm.h>
#include <h4/file.h>

int execvp(const char *path, char *const *argv);
int execv(const char *name, char *const *argv);
int exec(const char *name);
