#include "minix.h"
#include "dir.h"
#include "inode.h"
#include <stddef.h>
#include <assert.h>
#include <printk.h>

/* lookup a entry by name in specific dir inode */
struct inode *dir_lookup(struct inode *dip, const char *name, off_t *poff)
{
	off_t off;
	struct dir_entry de;

	printk("dir_lookup: inode-%d, lookup: %s\n", dip->ino, name);

	assert_info(S_ISDIR(dip->mode), "dir_lookup: no dir");

	for (off = 0; off < dip->size; off += sizeof(de)) {
		if (iread(dip, &de, off, sizeof(de)) != sizeof(de)) {
			printk(KL_ERROR "dir_lookup: fault when read");
			return NULL;
		}

		if (de.ino == 0)
			continue;

		printk("inode-%d named: [%s]\n", de.ino, de.name);

		if (!strncmp(name, de.name, NAME_LEN)) {
			printk("dir_lookup: found\n");

			/* found */
			if (poff)
				*poff = off;
			return iget(de.ino);
		}
	}

	printk("no found\n");
	/* no found */
	return NULL;
}

/* add a new dir entry into a directroy(dip)
 * increase nlinks of file(fip)
 * NB: fip must be lock and valid.
 */
bool dir_link(struct inode *dip, const char *name, struct inode *fip)
{
	off_t off;
	struct dir_entry de;
	struct inode *ip;

	printk("dir_link: link inode-%d named [%s] to dir inode-%d\n", fip->ino, name, dip->ino);

	assert_info(S_ISDIR(dip->mode), "dir_link: no dir");
	assert_info(fip->flags & I_BUSY, "dir_link: file inode no locked");
	assert_info(fip->flags & I_VALID, "dir_link: file inode no valid");

	/* add new dir entry */

	/* is this name existed? */
	if ((ip = dir_lookup(dip, name, 0))) {
		iput(ip); // dir_lookup return a inode by call iget()
		return false;
	}

	for (off = 0; off < dip->size; off += sizeof(de)) {
		if (iread(dip, &de, off, sizeof(de)) != sizeof(de)) {
			printk(KL_ERROR "dir_link: fault when read");
		}

		/* found a free entry */
		if (de.ino == 0)
			break;
	}
	strncpy(de.name, name, NAME_LEN);
	de.ino = fip->ino;

	if (iwrite(dip, &de, off, sizeof(de)) != sizeof(de)) {
		printk(KL_ERROR "dir_link: fault when write");
	}

	return true;
}

/* is directory empty?
 * only invoked when dip is locked */
bool dir_isempty(struct inode *dip)
{
	uint32_t off;
	struct dir_entry de;

	assert_info(S_ISDIR(dip->mode), "dir_isempty: no dir");

	/* skip "'."' and ".." */
	for (off = 2*sizeof(de); off < dip->size; off += sizeof(de)) {
		if (iread(dip, &de, off, sizeof(de) != sizeof(de))) {
			printk(KL_ERROR "dir_isempty: read fault");
			return false;
		}
		if (de.ino != 0)
			return false;
	}
	return true;
}
