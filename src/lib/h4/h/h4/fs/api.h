#pragma once

int open(const char *path, int flags);
int openat(int dirfd, const char *path, int flags);
int mkdir(const char *path, int mode);
int link(const char *oldpath, const char *newpath);
int symlink(const char *oldpath, const char *newpath);
int unlink(const char *path);
