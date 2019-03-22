#include "minix.h"
#include "file.h"
#include "stat.h"
#include "p2i.h"
#include "dir.h"
#include <stddef.h>
#include <printk.h>
#include <assert.h>
#include <errno.h>

#define NFILE 128

static struct file ftable[NFILE];
static int ftop = 0;

void ftable_init(void)
{
	memset(ftable, 0, sizeof(struct file)*NFILE);
}

struct file *falloc(void)
{
	int i;
	for (i = ftop; i < NFILE; i++) {
		if (!ftable[i].ref) {
			ftop = i;
			ftable[i].ref = 1;
			return &ftable[i];
		}
	}
	return NULL;
}

struct file *fdup(struct file *fp)
{
	assert_info(fp->ref >= 1, "fdup: file's ref < 1");

	fp->ref++;
	return fp;
}

void fclose(struct file *fp)
{
	assert_info(fp->ref >= 1, "fclose: file's ref < 1");

	fp->ref--;
	if (fp->ref > 0) {
		return;
	}

	if (fp->type == F_PIPE) {
		//pipe_close(fp->pipe, fp->writeable);
	} else if (fp->type == F_INODE) {
		iput(fp->ip);
	}

	fp->type = F_NONE;
	fp->ref = 0;

	if (fp - ftable < ftop)
		ftop = fp - ftable;
}

int fstat(struct file *fp, struct stat *st)
{
	if (fp->type == F_INODE) {
		ilock(fp->ip);
		istat(fp->ip, st);
		iunlock(fp->ip);

		return 0;
	}
	return -1;
}

int fread(struct file *fp, void *buf, size_t len)
{
	printk("fread: fp 0x%x, buf 0x%x, len %d ", fp, buf, len);
	if (!fp->readable) {
		printk("no readable");
		return -EPERM;
	}

	if (fp->type == F_PIPE) {
		printk("type: F_PIPE");
		return -ENOTSUP;//pipe_read(fp->pipe, buf, len);
	}

	if (fp->type == F_INODE) {
		printk("type: F_INODE");
		ilock(fp->ip);
		ssize_t ret = iread(fp->ip, buf, fp->off, len);
		if (ret < 0) {
			return ret;
		}
		fp->off += ret;
		iunlock(fp->ip);
	}

	return len;
}

int fwrite(struct file *fp, const void *buf, size_t len)
{
	printk("fwrite: fp 0x%x, buf 0x%x, len %d", fp, buf, len);

	if (!fp->writeable) {
		printk("fwrite: no writeable");
		return -EPERM;
	}

	if (fp->type == F_PIPE) {
		printk("fwrite: pipe");
		return -ENOTSUP;//pipe_write(fp->pipe, buf, len);
	}

	if (fp->type == F_INODE) {
		printk("fwrite: inode");
		ilock(fp->ip);
		ssize_t ret = iwrite(fp->ip, buf, fp->off, len);
		if (ret < 0) {
			printk(KL_WARN "fwrite: fault %d", ret);
			return ret;
		}
		fp->off += len;
		iunlock(fp->ip);

		return len;
	} else {

		printk(KL_ERROR "fwrite: wrong type");
		return -ESVRERR;
	}
}
