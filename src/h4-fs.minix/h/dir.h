#pragma once

struct inode *dir_lookup(struct inode *dip, char *name, off_t *poff);
bool dir_link(struct inode *dip, const char *name, struct inode *fip);
bool dir_isempty(struct inode *dip);
