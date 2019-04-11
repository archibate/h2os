#pragma once

int open(const char *path, unsigned int flags);
int openat(int dirfd, const char *path, unsigned int flags);
