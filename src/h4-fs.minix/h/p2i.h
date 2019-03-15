#pragma once

struct inode *p2ip(struct inode *cwd, const char *path, char *name);
struct inode *p2i(struct inode *cwd, const char *path);
