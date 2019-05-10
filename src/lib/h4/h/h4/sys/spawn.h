#pragma once

#include <h4/fs/defines.h>

#define MAX_ARGV   16
#define MAX_PERARG MAX_PATH
#define MAX_ENVP   32
#define MAX_PERENV MAX_PATH

struct spawnattr {
	int unused;
};

pid_t _spawn(const char *path, char *const *argv, char *const *envp, const struct spawnattr *sat);
