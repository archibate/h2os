// https://github.com/archibate/OS67/blob/master/fs/p2i.c
#include "minix.h"
#include "inode.h"
#include "p2i.h"
#include "dir.h"
#include "statmode.h"
#include <stddef.h>
#include <printk.h>
#include <string.h>
#include <errno.h>

static const char *skipelem(const char *path, char *name)
{
	while (*path == '/')
		path++;

	if (!*path)
		return NULL;

	const char *p = path;
	while (*p != '/' && *p)
		p++;

	size_t nlen = p - path;
	if (nlen >= NAME_LEN) {
		errno = ENAMETOOLONG;
		strncpy(name, path, NAME_LEN);
	} else {
		strncpy(name, path, nlen);
	}

	return path;
}

static struct inode *_path2inode(const char *path, bool parent, char *name)
{
	struct inode *ip, *next;

	if (*path == '/') {
		ip = iget(ROOT_INO);
	} else {
		assert_info(0, "cwd not supported");
		ip = NULL;
		//ip = idup(proc->cwd);
	}

	while ((path = skipelem(path, name))) {
		/* read from disk */
		ilock(ip);
		//print_i(ip);
		if (!S_ISDIR(ip->mode)) {
			iunlockput(ip);
			errno = ENOENT;
			return NULL;
		}

		if (parent && !*path) {
			iunlock(ip);
			return ip;
		}

		if (!(next = dir_lookup(ip, name))) {
			iunlockput(ip);
			errno = ENOENT;
			return NULL;
		}

		iunlockput(ip);
		ip = next;
	}

	if (parent) {
		iput(ip);
		errno = ENOTDIR;
		return NULL;
	}

	return ip;
}

struct inode *p2i(const char *path)
{
	char name[NAME_LEN];
	return _path2inode(path, 0, name);
}

struct inode *p2ip(const char *path, char *name)
{
	return _path2inode(path, 1, name);
}
