#pragma once

int fs_open(int fs, const char *path, unsigned int flags);
int fs_openat(int fs, int dirfd, const char *path, unsigned int flags);
