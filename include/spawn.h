#pragma once

#include <h4/sys/types.h>
#include <h4/sys/spawn.h>

pid_t spawnp(const char *path, char *const *argv, char *const *envp, const struct spawnattr *sat);
pid_t spawn(const char *name, char *const *argv, char *const *envp, const struct spawnattr *sat);
