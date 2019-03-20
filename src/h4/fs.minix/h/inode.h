#pragma once

struct stat;

struct inode *ialloc(void);
struct inode *iget(ino_t ino);
struct inode *idup(struct inode *ip);
void iput(struct inode *ip);
void ilock(struct inode *ip);
void iunlock(struct inode *ip);
void iunlockput(struct inode *ip);
void iupdate(struct inode *ip);
int iread (struct inode *ip, void *buf, size_t len, off_t off);
int iwrite(struct inode *ip, const void *buf, size_t len, off_t off);
void istat(struct inode *ip, struct stat *st);
void print_i(struct inode *ip);
